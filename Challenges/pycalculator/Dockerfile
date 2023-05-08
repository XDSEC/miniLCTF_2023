FROM ubuntu:22.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn/g" /etc/apt/sources.list && \
    apt-get update && apt-get -y dist-upgrade && \
    apt-get install -y python3 socat

RUN useradd -m ctf

WORKDIR /home/ctf

RUN mkdir /home/ctf/bin && \
    cp /bin/sh /home/ctf/bin && \
    cp /bin/ls /home/ctf/bin && \
    cp /bin/cat /home/ctf/bin && \
    cp /usr/bin/socat /home/ctf/bin

COPY ./start.sh /start.sh
COPY ./server.py /home/ctf/

RUN chown -R root:ctf /home/ctf && \
    chmod -R 750 /home/ctf && \
    chmod +x /home/ctf/server.py && \
    chmod +x /start.sh
CMD ["/start.sh"]

EXPOSE 9999
