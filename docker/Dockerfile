FROM ubuntu/mysql:latest as base

ARG TZ=CST

RUN apt-get update -qq && apt-get install -y  curl \
    && rm -rf /var/lib/apt/lists/*

############################################
# USER CONFIGURATION
############################################
FROM base as user

ARG USERNAME=ubuntu
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create a non-root user
RUN groupadd --gid $USER_GID $USERNAME \
  && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
  # Add sudo support for the non-root user
  && apt-get update \
  && apt-get install -y sudo \
  && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME\
  && chmod 0440 /etc/sudoers.d/$USERNAME \
  && rm -rf /var/lib/apt/lists/*

# Set up autocompletion for user
RUN apt-get update && apt-get install -y git-core bash-completion \
  && echo "if [ -f /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash ]; then source /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash; fi" >> /home/$USERNAME/.bashrc \
  && rm -rf /var/lib/apt/lists/* 

#####################################
### INSTALLING DOTNET 7
#####################################

ENV DOTNET_ROOT=/home/$USERNAME/dotnet
ENV PATH=$PATH:/home/$USERNAME/dotnet
ENV DOTNET_SYSTEM_GLOBALIZATION_INVARIANT=1

RUN cd /home/$USERNAME && curl -O https://download.visualstudio.microsoft.com/download/pr/dbfe6cc7-dd82-4cec-b267-31ed988b1652/c60ab4793c3714be878abcb9aa834b63/dotnet-sdk-7.0.400-linux-x64.tar.gz \
    && mkdir -p ./dotnet && tar zxf dotnet-sdk-7.0.400-linux-x64.tar.gz -C ./dotnet \
    && rm dotnet-sdk-7.0.400-linux-x64.tar.gz

## ENTRYPOINT FOR DOCKER
ENTRYPOINT [ "docker-entrypoint.sh" , "mysqld"]
