FROM debian:bookworm-20250317

RUN apt-get update
RUN apt-get install -y \
    build-essential git checkinstall libssl-dev openssl software-properties-common \
    golang-go \
    python3.11 python3-pip python3.11-venv libsass-dev libcairo2 libpango-1.0-0 libpangoft2-1.0-0 pangocairo-1.0 pngquant \
    sudo

## vscode user
RUN useradd -ms /bin/bash vscode \
    && apt-get update \
    && apt-get install -y sudo \
    && echo vscode ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/vscode \
    && chmod 0440 /etc/sudoers.d/vscode

USER vscode
WORKDIR /home/vscode

## Python will run in venv
RUN python3 -m venv /home/vscode/venv
ENV PATH="/home/vscode/venv/bin:$PATH"

