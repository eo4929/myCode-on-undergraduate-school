var express = require('express');
var enroll = require('../fabric_js/enrollAdmin.js')
var register = require('../fabric_js/registerUser.js')
var query = require('../fabric_js/query.js')
var invoke = require('../fabric_js/invoke.js')
var router = express.Router();

let user;
let str_mycar;
let str_allRegCar;
let str_sellmycar;
let str_buycar;

/* GET home page. */
router.get('/', async function(req, res, next) {
	console.log('hi');
	user = req.cookies.user;
	if(user != ''){
		str_mycar = await query.query(user,"getMyCar"); console.log(str_mycar); //res.cookie('mycar', str_mycar);
		//becaouse of server down, comment out //str_allRegCar = await query.query(user,"getAllRegisteredCar"); console.log(str_allRegCar);//res.cookie('allRegCar', str_allRegCar);
		//becaouse of server down, comment out //str_allOrderedCar = await query.query(user,"getAllOrderedCar"); //res.cookie('allOrderedCar', str_allOrderedCar);

		str_sellmycar = await query.query(user,"getSellMyCar"); //res.cookie('sellmycar', str_sellmycar);
		//becaouse of server down, comment out //str_buycar = await query.query(user,"getBuyCar"); //res.cookie('buycar', str_buycar);
	}
	//res.render('index', { name: req.cookies.user ,myCars: str_mycar, registeredCars: str_allRegCar, carsOnSale: str_allOrderedCar, mycarscategory: str_sellmycar, salecategory: str_buycar});
	res.render('index', { name: req.cookies.user ,myCars: str_mycar, registeredCars: "because of server down, comment out(20line in index.js)", carsOnSale: "because of server down, comment out(21line in index.js)", mycarscategory: str_sellmycar, salecategory: "because of server down, comment out(24line in index.js)"}); 	
});

router.get('/enrollAdmin', async function(req, res, next) {
	await enroll.enrollAdmin();
		res.redirect('/');
})

router.post('/registerUser', function(req, res, next) { 
	user = req.body.user;
	console.log(user);
	register.registerUser(user);
	res.cookie('user', user);

	res.redirect('/');
})

router.get('/registerCar', async function(req, res, next) {
	user = req.cookies.user;
	
	make = req.query.make; // make = req.body.make;
	model = req.query.model;
	color = req.query.color;
	console.log(make);
	await invoke.invoke(user,"registerCar", [make, model, color] );
	
	res.redirect('/');
})

router.get('/sellMyCar', async function(req, res, next) { //becaouse of server down, can not test
	user = req.cookies.user;

	cid = req.query.sid;
	price = req.query.price;

	console.log(price);
	await invoke.invoke(user,"sellMyCar", [cid,price] );
	
	res.redirect('/');
})

router.get('/buyCar', async function(req, res, next) { //becaouse of server down, can not test
	user = req.cookies.user;

	cid = req.query.sid2;

	console.log(prevOwner);
	await invoke.invoke(user,"buyUserCar", [cid] );
	
	res.redirect('/');
})

// example, how to use json object in router
// router.post('/query', async function(req, res, next) {
// 	console.log('name : ', user)
// 	var result = await query.query(req.cookies.user)
// 	console.log('result : ', result)
// 	console.log('make : ', result['make'])

// 	res.redirect('/');
// })


module.exports = router;
