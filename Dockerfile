# Use an official Ubuntu as a parent image
FROM ubuntu:20.04

# Set environment variables to non-interactive for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary tools and dependencies
RUN apt-get update && apt-get install -y \
    sudo \
    git \
    build-essential \
    software-properties-common \
    curl \
    cmake \
    libssl-dev \
    libcurl4-openssl-dev \
    libgtest-dev \
    gdb \
    clang-format

# Add the pistache PPA and install pistache
RUN sudo add-apt-repository ppa:pistache+team/unstable && \
    sudo apt-get update && \
    sudo apt-get install -y libpistache-dev

# Clone the cpp-api-ems repository
RUN git clone https://github.com/jvlavan/cpp-api-ems.git

# Set the working directory
WORKDIR /cpp-api-ems

# Compile the C++ project with C++17 standard
RUN mkdir -p build && \
    cd build && \
    cmake -DCMAKE_CXX_STANDARD=17 .. && \
    make

# Install Node.js and http-server to serve index.html
RUN curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash - && \
    sudo apt-get install -y nodejs && \
    sudo npm install -g http-server

# Expose port 9082
EXPOSE 9082

# Start the C++ application and serve the index.html file
CMD ./build/ems-rest-final & http-server public -p 9082
