use std::fs::File;
use bytes::Bytes;
use http_body_util::{combinators::BoxBody, BodyExt};
use hyper::{Request, Response, StatusCode};
use hyper::body::Incoming;
use std::io::Write;
use crate::util::empty;

pub async fn upload(req: Request<Incoming>) -> Response<BoxBody<Bytes, hyper::Error>> {
    if let Some(filename) = req.headers().get("filename") {
        if let Ok(filename) = filename.to_str() {
            if let Ok(mut file) = File::create(filename) {
                let body = req.collect().await.unwrap().to_bytes();
                // println!("{:?}", body);
                if let Ok(()) = file.write_all(&body) {
                    let mut resp = Response::new(empty());
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