FROM node:18 AS base
WORKDIR /crypt3

COPY ./package.json ./yarn.lock /crypt3
RUN yarn install --ignore-scripts --frozen-lockfile
COPY ./binding.cc ./binding.gyp /crypt3
RUN yarn install

COPY ./ /crypt3
RUN yarn test

ENTRYPOINT ["/bin/bash"]