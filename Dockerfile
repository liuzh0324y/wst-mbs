FROM ubuntu:16.04

COPY ./build/wst-mbs /usr/bin

WORKDIR /usr/bin

CMD ["./wst-mbs"]