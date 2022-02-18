FROM gcc:latest

WORKDIR /usr/app
COPY ./ .
RUN mkdir -p premake

WORKDIR /usr/app/premake
RUN wget --no-check-certificate https://github.com/premake/premake-core/releases/download/v5.0.0-beta1/premake-5.0.0-beta1-linux.tar.gz && \
 tar xvzf premake-5.0.0-beta1-linux.tar.gz && \
 rm premake-5.0.0-beta1-linux.tar.gz
 
ENV OLDPATH="${PATH}"
ENV PATH="/usr/app/premake:${OLDPATH}"

WORKDIR /usr/app/sln
RUN apt install libx11-dev
RUN premake5 gmake2 && make
RUN apt update
RUN apt upgrade -y
RUN apt dist-upgrade -y
RUN apt install gdb -y

CMD /usr/app/Test/build/Debug/Test
