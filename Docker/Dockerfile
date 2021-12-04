
FROM python:3

RUN apt update && \
    apt install -y nautilus && \
    pip install --upgrade pip && \
    pip install numpy scipy matplotlib openpyxl pandas

WORKDIR /home