pragma solidity ^0.5;
pragma experimental ABIEncoderV2;

contract ICarTrade {
    struct Car{
        uint number;
        string owner_name;
        string make;
        string model;
        string color;
        address payable owner;
    }
    
    struct Order{
        Car car;
        uint price;
        string status;
    }
    
    uint256 cid=0; // means unique id = cnumber
    Car[] carList;
    Order[] orderList;
    mapping(address=>string) nameOf;
    
    function registerCar(string memory make, string memory model, string memory color) public{
        Car memory newcar;
        newcar.number = cid; cid++;
        newcar.owner_name = nameOf[msg.sender];
        newcar.make = make; newcar.model = model; newcar.color = color;
        newcar.owner = msg.sender;
        
        carList.push(newcar);
    }
    function registerName(string memory name) public{
        nameOf[msg.sender] = name;
        for(uint256 i=0; i<carList.length; i++){
            if( carList[i].owner == msg.sender){
                carList[i].owner_name = nameOf[msg.sender];
            }
        }
        for(uint256 i=0; i<orderList.length; i++){
            if(orderList[i].car.owner == msg.sender){
                orderList[i].car.owner_name = nameOf[msg.sender];
            }
        }
    }
    function sellMyCar(uint cnumber, uint price) public{
        Order memory neworder;
        for(uint256 i=0; i<carList.length; i++){
            if( carList[i].owner == msg.sender && cnumber == carList[i].number){
                neworder.car = carList[i];
                break;
            }
        }
        neworder.price = price;
        neworder.status = "sale";
        
        orderList.push(neworder);
    }
    function buyUserCar(uint orderedcnumber) payable public{
        for(uint256 i=0; i<orderList.length; i++){
            if(orderedcnumber == orderList[i].car.number){
                changeCarOwner(orderedcnumber,orderList[i].car.owner);
                balanceTransfer(orderList[i].car.owner,orderList[i].price);
                
                orderList[i].status = "done";
                break;
            }
        }
    }
    function balanceTransfer(address payable seller, uint price) payable public{
        price = price * 10**18;
        seller.transfer(price);
    }
    function changeCarOwner(uint cnumber, address addr) public{
        for(uint256 i=0; i<carList.length; i++){
            if(cnumber == carList[i].number && addr == carList[i].owner ){
                carList[i].owner = msg.sender;
                carList[i].owner_name = nameOf[msg.sender];
                break;
            }
        }
    }
    function getMyCars() public view returns(Car[] memory){
        uint256 size = 0;
        for(uint256 i=0; i<carList.length; i++){
            if(carList[i].owner == msg.sender){
                size++;
            }
        }
        Car[] memory mycars = new Car[](size);
        uint256 j=0;
        for(uint256 i=0; i<carList.length; i++){
            if(carList[i].owner == msg.sender){
                mycars[j] = carList[i];
                j++;
            }
        }
        return mycars;
    }
    function getName() public view returns(string memory){
        return nameOf[msg.sender];
    }
    function getAllRegisteredCar() public view returns(Car[] memory){
        return carList;
    }
    function getAllOrderedCar() public view returns(Order[] memory){
        return orderList;
    }
    
    
}


