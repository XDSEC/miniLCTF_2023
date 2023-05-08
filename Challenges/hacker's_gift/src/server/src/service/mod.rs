mod getkey;
mod upload;

use bytes::Bytes;
use http_body_util::combinators::BoxBody;
use hyper::{Request, Response, StatusCode};
use crate::service::getkey::get_key;
use crate::service::upload::upload;
use crate::util::empty;

pub async fn preprocess(
    req: Request<hyper::body::Incoming>,
) -> Result<Response<BoxBody<Bytes, hyper::Error>>, hyper::Error> {
    let mut resp_notfound = Response::new(empty());
    *resp_notfound.status_mut() = StatusCode::NOT_FOUND;
    let mut resp_forbidden = Response::new(empty());
    *resp_forbidden.status_mut() = StatusCode::FORBIDDEN;
    // check client User-Agent
    if let Some(ua) = req.headers().get("User-Agent") {
        if ua.to_str().unwrap_or("") != "Mini-L-CTF-2023" {
            return Ok(resp_forbidden)
        }
    }
    // router
    let resp = match req.uri().path().to_string().as_str() {
        "/getkey" => get_key(req).await,
        "/upload" => upload(req).await,
        _ => resp_notfound
    };
    Ok(resp)
}
