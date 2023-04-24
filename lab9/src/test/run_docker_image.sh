#!/bin/bash

# set colors
CLR="\033[38;5;117m"
RST="\033[0m"

# start docker if is not running yet
if ! docker ps >/dev/null; then
  if [ "$OS" = 'macOS' ]; then
    echo "Starting Docker app... Delay is 15 sec"
    open -a Docker
    sleep 10
  else
    echo "you should run Docker first"
    exit 0
  fi
fi

# set default image if another is not specified
if test -z "$image"; then
  image_os="ubuntu"
else
  image_os="$image"
fi

container_name="dondarri.${image_os}-temp"
docker rm -f ${container_name} #2> /dev/null
dockerfile="Dockerfile.$image_os"
image="dondarri/$image_os"
prompt="$CLR$image_os@container$RST:\W$ "
command="echo \"export PS1='$prompt'\" >> ~/.bashrc && bash"

docker build -t $image -f test/$dockerfile .
docker run -it \
  --name "$container_name" \
  -e PS1="$prompt" \
  -v $PWD:/usr/project \
  -w /usr/project \
  $image \
  bash -c "$command"

docker rm ${container_name} > /dev/null
echo "docker: container removed"
make clean