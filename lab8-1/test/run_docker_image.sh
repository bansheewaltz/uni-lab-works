#!/bin/bash

# set colors
CLR="\033[38;5;117m"
RST="\033[0m"

# start docker if is not running yet
if ! docker ps >/dev/null; then
  if [ "$OS" = 'macOS' ]; then
    echo "Starting Docker app... Delay is 17 sec"
    open -a Docker
    sleep 17
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

# set default platform if another is not specified
if test -z "$platform"; then
  platform_flag=
else
  platform_flag="--platform $platform"
fi

container_name="dondarri.${image_os}-temp"
docker rm -f ${container_name} 2> /dev/null
dockerfile="Dockerfile.$image_os"
image_name="dondarri/$image_os"
prompt="$CLR$image_os@container$RST:\W$ "
command="echo \"export PS1='$prompt'\" >> ~/.bashrc && bash"


docker build -t $image_name $platform_flag -f ${TEST_DIR}/$dockerfile .
docker run -it \
  --rm \
  $platform_flag \
  --name "$container_name" \
  -e PS1="$prompt" \
  -v $PWD/../:/usr/project \
  -w /usr/project/${LAB} \
  $image_name \
  bash -c "$command"

echo "docker: container removed"
if [ "$image" != "native" ]; then
  make clean
fi
