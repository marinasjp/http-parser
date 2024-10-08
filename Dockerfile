FROM gcc:latest

COPY . /httpparser
WORKDIR /httpparser

RUN gcc --version
RUN gcc -I. -o server ./server.cpp ./tcpServer.cpp -lstdc++

CMD [ "./server"] 

EXPOSE 8080
