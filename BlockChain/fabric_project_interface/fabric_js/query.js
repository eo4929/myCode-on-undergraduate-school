/*
 * SPDX-License-Identifier: Apache-2.0
 */

'use strict';

const { FileSystemWallet, Gateway } = require('fabric-network');
const path = require('path');

const ccpPath = path.resolve(__dirname, '..', 'fabric-network', 'connection-org1.json');

var query = async function(name,fun,args){
    try {

        // Create a new file system based wallet for managing identities.
        const walletPath = path.join(process.cwd(), 'wallet');
        const wallet = new FileSystemWallet(walletPath);
        console.log(`Wallet path: ${walletPath}`);

        // Check to see if we've already enrolled the user.
        const userExists = await wallet.exists(name);
        if (!userExists) {
            console.log('An identity for the user does not exist in the wallet');
            console.log('Run the registerUser.js application before retrying');
            return;
        }

        // Create a new gateway for connecting to our peer node.
        const gateway = new Gateway();
        await gateway.connect(ccpPath, { wallet, identity: name, discovery: { enabled: true, asLocalhost: true } });

        // Get the network (channel) our contract is deployed to.
        const network = await gateway.getNetwork('mychannel');

        // Get the contract from the network.
        const contract = network.getContract('mycc');

        //execute query function
        //const result = await contract.evaluateTransaction('query', 'a');
        var result = '';
        var res_str = '';

	if(fun == "getMyCar"){
            console.log('arrive at getMyCar');
            result = await contract.evaluateTransaction('getMyCar', name, "getMyCar");
            console.log(`Transaction has been evaluated, result is: ${result.toString()}`);
            //return result.toString();
        }
        else if(fun == "getAllRegisteredCar"){ //becaouse of server down, can not test
            console.log('arrive at getAllRegisteredCar');
            result = await contract.evaluateTransaction('getAllRegisteredCar',"func1");
            console.log(`Transaction has been evaluated, result is: ${result.toString()}`);
            //return result.toString();
        }
        else if(fun == "getAllOrderedCar"){ //becaouse of server down, can not test
            result = await contract.evaluateTransaction('getAllOrderedCar');
            console.log(`Transaction has been evaluated, result is: ${result.toString()}`);
            //return result.toString();
        }
        else if(fun == "getSellMyCar"){
            console.log('arrive at getSellMyCar');
            result = await contract.evaluateTransaction('getMyCar', name, "getSellMyCar");
            console.log(`Transaction has been evaluated, result is: ${result.toString()}`);
            //return result.toString();
        }
        else if(fun == "getBuyCar"){ //becaouse of server down, can not test
            result = await contract.evaluateTransaction('getAllRegisteredCar',"func2");
            console.log(`Transaction has been evaluated, result is: ${result.toString()}`);
            //return result.toString();
        }
        //query result value(string), string to json and use
        //var test = JSON.parse(result);  -> string to json
        //console.log('test: ', test['make']);  -> use json object
        //console.log(`Transaction has been evaluated, result is: ${result.toString()}`);
        /*
        for(key in json) {
        alert('key:' + key + ' / ' + 'value:' + json[key]);
        }    */
        var res_str = '';
        var res_json = '';

        if(fun == "getMyCar" || fun == "getSellMyCar"){
            res_str = result.toString();
            res_json = JSON.parse(result.toString());
            console.log( JSON.stringify(res_json) );

            res_str = '';
            for( key in res_json){
                res_str += (res_json[key] + '\t');
            }
        }
        else{ //becaouse of server down, can not test // other 3 func move to this line which are related to 'get some cars' 
            res_str = result.toString();
            //res_json = JSON.parse(result.toString()); // need to checking;; => if there is a problem, use res_str = result.toString
            //res_str = JSON.stringify(res_json);
            //for(var ele in res_json){
            //    res_str += ("   each car: "+ res_json[ele]);
            //}
        }
        
        /*
        res_str = '';
        for(var i=0; i<res_json.length; i++){
            console.log(res_json[i]['make']);
            res_str += res_json[i]['make'] + '\t' + res_json[i]['model'] + '\t' + res_json[i]['color']; // nono! res_str is str , res_json is JOSN
            res_str += '\n';
        }
         */
        
        return res_str.toString();

    } catch (error) {
        console.error(`Failed to evaluate transaction: ${error}`);
        process.exit(1);
    }
}

exports.query = query;
