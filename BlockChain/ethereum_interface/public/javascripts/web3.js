var web3 = new Web3();
web3.setProvider(new Web3.providers.HttpProvider('http://localhost:8545'));

const contract_address = "";
const abi = 

let carTrade = new web3.eth.Contract(abi, contract_address);
var active_addr = '';

$(document).ready(function() {
	active_addr = $('#address').text();
	//alert(active_addr);
	startDapp();
})

var startDapp = async function() {
	//alert(active_addr);
	printMe();
	//getMyCars();
	//getRegisteredCars();
	//getSellMyCars();
	//getCarsOnSale();
	//getBuyUsersCar();
}

var printMe = async function() {
	if(active_addr == ''){
	}
	else{
		getBalance();
		getName();
		getMyCars();
		getRegisteredCars();
		getCarsOnSale();
	}
}

var getBalance = async function() { // change ether unit correctly
	//alert('getBalance');
	html = "";
	//var mybal = await carTrade.methods.getMybalance().call({from :active_addr});
	var mybal = await web3.eth.getBalance(active_addr);
	//html += carTrade.methods.getMybalance().call({from :active_addr});
	//alert(html);
	//console.log(html);
	//document.getElementById('balanceAmount').innerHTML = html;
	//alert(mybal);
	mybal = mybal / Math.pow(10, 18);
	$('#balanceAmount').text(mybal);
	
	//startDapp();
}

var getName = async function() {
	//alert('getName');
	//html = "";
	//html += await carTrade.methods.getName().call({from :active_addr});
	//alert(html);
	//document.getElementById('name').innerHTML = html;
	var myname = await carTrade.methods.getName().call({from:active_addr, gas:3000000});
	$('#name').text(myname);
	//alert(myname);
	//startDapp();
}

var registerName = async function() {
	if(active_addr == ''){
		alert('At first, click CHANGE USER for selecting a user');
		return;
	}
	var newname = await document.getElementById("change_name").value;
	await carTrade.methods.registerName(newname).send({from: active_addr, gas:5000000});
	printMe();
}

var changeUser = async function(){
	var newuser = await document.getElementById("private_key").value;
	$('#address').text(newuser);
	active_addr = $('#address').text();
	//alert(active_addr);
	printMe();
}

var registerMyCar = async function() {
	
	var make = await document.getElementById("make").value;
	var model = await document.getElementById("model").value;
	var color = await document.getElementById("color").value;
	
	await carTrade.methods.registerCar(make,model,color).send({from: active_addr, gas:3000000});
	printMe();
}

var sellMyCar = async function() { // for onclick
	var sid = await document.getElementById("sid").value;
	var price = await document.getElementById("price").value;

	await carTrade.methods.sellMyCar(sid,price).send({from: active_addr, gas:3000000});
	printMe();
}

var buyUserCar = async function() { // for onclick
	var sid2 = await document.getElementById("sid2").value;
	var price2 = await document.getElementById("price2").value;

	await carTrade.methods.buyUserCar(sid2).send({from : active_addr, gas:5000000, value:price2 * Math.pow(10, 18)});
	printMe();
}

var getMyCars = async function() {
	var i=0; var j=0;
	var cars = await carTrade.methods.getMyCars().call({from: active_addr, gas:3000000});
	var orders = await carTrade.methods.getAllOrderedCar().call({from: active_addr, gas:3000000});

	var mytbody = await document.getElementById("myCars");
	mytbody.innerHTML = '';
	
	//alert(cars);
	for(i=0; i<cars.length; i++){
		var number = cars[i][0]; // 0
		var owner_name = cars[i][1]; // 1
		var make=cars[i][2]; 
		var model=cars[i][3];
		var color= cars[i][4];

		var row = mytbody.insertRow(mytbody.rows.length); //mytbody.rows.length -> like EOF
		var col0 = row.insertCell(0);
		var col1 = row.insertCell(1);
		var col2 = row.insertCell(2);
		var col3 = row.insertCell(3);
		var col4 = row.insertCell(4);

		col0.innerHTML = number; col1.innerHTML = owner_name; col2.innerHTML = make; col3.innerHTML = model; col4.innerHTML = color; 
		//$(myCars).find('tbody').append("<tr> cars[0]['number'] </tr>");
	}

	var mytbody2 = await document.getElementById("mycars-category");
	mytbody2.innerHTML = '';
	for(i=0; i<cars.length; i++){

		var number = cars[i][0]; // 0
		var owner_name = cars[i][1]; // 1
		var make=cars[i][2]; 
		var model=cars[i][3];
		var color= cars[i][4];	

		var row = mytbody2.insertRow(mytbody2.rows.length); //mytbody.rows.length -> like EOF
		var col0 = row.insertCell(0);
		var col1 = row.insertCell(1);
		var col2 = row.insertCell(2);
		var col3 = row.insertCell(3);
		var col4 = row.insertCell(4);

		col0.innerHTML = number; col1.innerHTML = owner_name; col2.innerHTML = make; col3.innerHTML = model; col4.innerHTML = color; 
		
		for(j=0; j<orders.length; j++){
			//alert(orders[j][0][0]);
			if(cars[i][0] == orders[j][0][0] && orders[j][2] == "sale" ){
				//alert(j);
				mytbody2.deleteRow(mytbody2.rows.length-1);
				break;
			}
		}
		
		//$(myCars).find('tbody').append("<tr> cars[0]['number'] </tr>");
	}
	//mytbody.deleteRow(mytbody.rows.length-1);
	//alert(cars);
	//alert(rows);

	var mytbody3 = await document.getElementById("sale-category");
	mytbody3.innerHTML = '';
	
	for(i=0; i<orders.length; i++){
		if( orders[i][2] != "done"){
			
			var number = orders[i][0][0]; // 0
			var owner_name = orders[i][0][1]; // 1
			var make=orders[i][0][2];
			var model=orders[i][0][3];
			var color= orders[i][0][4];
			var price = orders[i][1];

			var row = mytbody3.insertRow(mytbody3.rows.length); //mytbody.rows.length -> like EOF
			var col0 = row.insertCell(0);
			var col1 = row.insertCell(1);
			var col2 = row.insertCell(2);
			var col3 = row.insertCell(3);
			var col4 = row.insertCell(4);
			var col5 = row.insertCell(5);

			col0.innerHTML = number; col1.innerHTML = owner_name; col2.innerHTML = make; col3.innerHTML = model; col4.innerHTML = color;
			col5.innerHTML = price;
		
		//$(myCars).find('tbody').append("<tr> cars[0]['number'] </tr>");
			/*
			for(j=0; j<cars.length; j++){
			//alert(orders[j][0][0]);
			if(cars[j][0] == orders[i][0][0] ){
				//alert(j);
				mytbody3.deleteRow(mytbody3.rows.length-1);
				break;
			}
		}*/

		}
		
		
	}
}

var getRegisteredCars = async function() {
	var mytbody = await document.getElementById("registeredCars");
	mytbody.innerHTML = '';
	var registeredCars = await carTrade.methods.getAllRegisteredCar().call({from:active_addr, gas:3000000});
	var i =0;
	for(i=0; i<registeredCars.length; i++){
		var number = registeredCars[i][0]; // 0
		var owner_name = registeredCars[i][1]; // 1
		var make=registeredCars[i][2]; 
		var model=registeredCars[i][3];
		var color= registeredCars[i][4];	

		var row = mytbody.insertRow(mytbody.rows.length); //mytbody.rows.length -> like EOF
		var col0 = row.insertCell(0);
		var col1 = row.insertCell(1);
		var col2 = row.insertCell(2);
		var col3 = row.insertCell(3);
		var col4 = row.insertCell(4);

		col0.innerHTML = number; col1.innerHTML = owner_name; col2.innerHTML = make; col3.innerHTML = model; col4.innerHTML = color;

	}
	
}

var getSellMyCars = async function() { 
	// be implemented by using func getMyCars
}

var getCarsOnSale = async function() { 
	var mytbody = await document.getElementById("carsOnSale");
	mytbody.innerHTML = '';
	var carsOnSale = await carTrade.methods.getAllOrderedCar().call({from:active_addr, gas:3000000});
	var i =0;
	for(i=0; i<carsOnSale.length; i++){
		var number = carsOnSale[i][0][0]; // 0
		var owner_name = carsOnSale[i][0][1]; // 1
		var make=carsOnSale[i][0][2]; 
		var model=carsOnSale[i][0][3];
		var color= carsOnSale[i][0][4];
		var price = carsOnSale[i][1];
		var status = carsOnSale[i][2];

		var row = mytbody.insertRow(mytbody.rows.length); //mytbody.rows.length -> like EOF
		var col0 = row.insertCell(0);
		var col1 = row.insertCell(1);
		var col2 = row.insertCell(2);
		var col3 = row.insertCell(3);
		var col4 = row.insertCell(4);
		var col5 = row.insertCell(5);
		var col6 = row.insertCell(6);

		col0.innerHTML = number; col1.innerHTML = owner_name; col2.innerHTML = make; col3.innerHTML = model; col4.innerHTML = color;
		col5.innerHTML = price; col6.innerHTML = status;

		//if(carsOnSale[i][2] == "done" ){
		//	//alert(j);
		//	mytbody.deleteRow(mytbody.rows.length-1);
		//}

	}
}

var getBuyUsersCar = async function() {
	// be implemented by using func getMyCars
}

