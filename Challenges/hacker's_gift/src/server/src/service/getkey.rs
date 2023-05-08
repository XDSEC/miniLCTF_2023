use rsa::{RsaPublicKey, Pkcs1v15Encrypt, PublicKey};
use rsa::pkcs8::DecodePublicKey;
use rand;
use bytes::Bytes;
use http_body_util::combinators::BoxBody;
use hyper::{Request, Response, StatusCode};
use hyper::body::Incoming;
use crate::util::{empty, full};
// rsa 2048 bit 
pub async fn get_key(req: Request<Incoming>) -> Response<BoxBody<Bytes, hyper::Error>> {
    let key = rand::random::<u8>().to_string() + "1e07f177-e9e8-494d-bd46-ab791cb4c694";
    // if let Ok(mut file) = File::open("./key") {
    //     file.read_to_string(&mut key).unwrap();
    // } else {
    //     key = "1e07f177-e9e8-494d-bd46-ab791cb4c694".to_string();
    // }
    if let Some(public_key) = req.headers().get("key") {
        // println!("{:?}", public_key);
        if let Ok(public_key) = public_key.to_str() {
            if public_key.len() == 392 {
                let public_key = "-----BEGIN PUBLIC KEY-----\n".to_string()
                + &public_key[0..64] + "\n"
                + &public_key[64..128] + "\n"
                + &public_key[128..192] + "\n"
                + &public_key[192..256] + "\n"
                + &public_key[256..320] + "\n"
                + &public_key[320..384] + "\n"
                + &public_key[384..] + "\n"
                + "-----END PUBLIC KEY-----";
                // println!("{}", public_key);
                // println!("{}", public_key.len());
                if let Ok(public_key) = RsaPublicKey::from_public_key_pem(&public_key) {
                    let mut rng = rand::thread_rng();
                    // Encrypt
                    let enc_data = public_key.encrypt(&mut rng, Pkcs1v15Encrypt, key.as_ref()).unwrap();
                    // println!("{:?}\n{:?}", key, enc_data);
                    let mut resp = Response::new(full(enc_data.clone()));
                    *resp.status_mut() = StatusCode::OK;
                    return resp
                }
            }
        }
    }
    let mut resp = Response::new(empty());
    *resp.status_mut() = StatusCode::BAD_REQUEST;
    resp
}
