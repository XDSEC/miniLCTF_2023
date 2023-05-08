mod service;
mod util;

use hyper::server::conn::http1;
use hyper::service::service_fn;
use tokio::net::TcpListener;
use service::preprocess;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error + Send + Sync>> {
    let addr = "0.0.0.0:8080";
    let listener = TcpListener::bind(&addr).await?;
    println!("listen on {:?}", addr);

    loop {
        let (stream, addr) = listener.accept().await?;
        println!("connect to {}", addr);

        tokio::spawn(async move {
            if let Err(err) = http1::Builder::new()
                .serve_connection(stream, service_fn(preprocess))
                .await
            {
                println!("Error serving connection: {:?}", err);
            }
        });
    }
}
