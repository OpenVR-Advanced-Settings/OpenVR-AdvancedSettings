# CI Build Images for OVRAS

Docker images for use on CI.

Can be built with `sudo docker build -t [tagname] . -f DockerfileName`.

Must be uploaded to DockerHub through `sudo docker push [tagname]` and then used in the CI file via the `image: ` command.


