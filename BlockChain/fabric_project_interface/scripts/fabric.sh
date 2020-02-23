
# fabric network generate && network start
cd ~/fabric_project/fabric-network
./byfn.sh -m generate
./byfn.sh -m up -a -n -s couchdb

# fabric network down && container down && check docker container
cd ~/fabric_project/fabric-network
./byfn -m down 
|
IMAGE_TAG=latest docker-compose -f docker-compose-cli.yaml down --volumes
docker rm -f $(docker ps -aq)
docker network prune
docker ps

# execute command line interface container
docker exec -it cli bash

# chaincode install
peer chaincode install -n mycc -v 1.0 -l java -p /opt/gopath/src/github.com/chaincode/

# check chaincode install
peer chaincode list --installed

# instantiate chaincode
peer chaincode instantiate -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -l java -v 1.0 -c '{"function":"init","Args":[]}'	

# invoke chaincode => when call chaincode function, use this sentences, not use query below
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"registerCar", "Args":["321", "321", "321", "zzzzz"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"registerCar", "Args":["441", "441", "441", "wewew"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"getMyCar", "Args":["zzzzz","getMyCar"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"getAllRegisteredCar", "Args":["func1"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"sellMyCar", "Args":["CAR001","43"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"buyUserCar", "Args":["CAR001","zzzzzzzzzzzzzz"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"getAllRegisteredCar", "Args":["func1"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"getMyCar", "Args":["wewew","getSellMyCar"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"sellMyCar", "Args":["CAR002","77"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"getAllRegisteredCar", "Args":["func2"]}'
peer chaincode invoke -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C $CHANNEL_NAME -n mycc -c '{"function":"getAllOrderedCar", "Args":[]}'


# query chaincode
#peer chaincode query -C mychannel -n mycc -c '{"Args":["query","a"]}'
peer chaincode query -C mychannel -n mycc -c '{"function":"getMyCar", "Args":[]}'

# chaincode upgrade(버전을 다르게 지정)
peer chaincode upgrade -o orderer.example.com:7050 --tls --cafile $ORDERER_CA -C mychannel -n mycc -l java -v 1.2 -c '{"function":"init","Args":[]}'



