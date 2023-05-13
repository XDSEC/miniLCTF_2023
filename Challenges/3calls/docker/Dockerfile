FROM ubuntu:22.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn/g" /etc/apt/sources.list && \
    apt-get update && apt-get -y dist-upgrade && \
    apt-get install -y lib32z1 xinetd

RUN apt-get install -y binutils grep gawk

RUN useradd -m ctf

WORKDIR /home/ctf

RUN cp -R /usr/lib* /home/ctf

RUN mkdir /home/ctf/bin && \
    cp /bin/sh /home/ctf/bin && \
    cp /bin/ls /home/ctf/bin && \
    cp /bin/cat /home/ctf/bin

RUN cp /bin/readelf /home/ctf/bin && \
    cp /bin/tail /home/ctf/bin && \
    cp /bin/grep /home/ctf/bin && \
    cp /bin/awk /home/ctf/bin

COPY ./ctf.xinetd /etc/xinetd.d/ctf
COPY ./start.sh /start.sh
RUN echo "Blocked by ctf_xinetd" > /etc/banner_fail

RUN chmod +x /start.sh

COPY ./pwn /home/ctf/
COPY ./libc.so.6 /home/ctf/
RUN chown -R root:ctf /home/ctf && \
    chmod -R 750 /home/ctf

CMD ["/start.sh"]

EXPOSE 9999
