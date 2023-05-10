FROM python:3.8-alpine
LABEL Description="guess" VERSION='1.0'

RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories && apk update && apk add gcc g++ make openssl-dev python3-dev libffi-dev autoconf && mkdir -p /opt/aes && pip install pycryptodome gmpy2 -i https://pypi.mirrors.ustc.edu.cn/simple 


WORKDIR /opt/Guess

COPY task.py .
COPY secret.py .

EXPOSE 10001
CMD ["python", "-u", "task.py"]
