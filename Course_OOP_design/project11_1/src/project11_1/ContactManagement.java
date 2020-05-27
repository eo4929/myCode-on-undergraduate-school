package project11_1;

import java.util.Scanner;

public class ContactManagement {
	public static void main(String[] args) {
	     
		GenericManager<Contact> contactManager = new GenericManager<Contact>(); 
	    boolean exit = false;
	    Scanner keyboard = new Scanner(System.in); 
	     
	     do{
		     //print the menu
		     System.out.println("*******Contact Management System*******"); 
		     System.out.println("(1) Add a contact: ");
		     System.out.println("(2) Display all contacts: ");
		     System.out.println("(3) Search for contacts");
		     System.out.println("(4) Sort the contacts");
		     System.out.println("(5) Exit the program");
		     System.out.println("***************************************");
		     //using an Exception control loop
		     try{
		          int input = keyboard.nextInt();
		          switch(input) {
		          case 1:
		        	  System.out.println("Please enter the name:");
		        	  String name = keyboard.next();
		        	  name += keyboard.nextLine();
		        	  System.out.println("Please enter the tell#:");
		        	  String tel = keyboard.next();
		        	  System.out.println("Please enter the email:");
		        	  String email = keyboard.next();
		        	  contactManager.add(new Contact(name,tel,email));
		        	  break;
		          case 2:
		        	  System.out.println("**********Contact List**********"); 
		        	  System.out.println(contactManager.toString()); 
		        	  System.out.println("********************************"); 
		        	  break;
		          case 3:
		        	  System.out.println("Please enter the name to search for:"); 
		        	  String aname = keyboard.next(); 
		        	  aname += keyboard.nextLine(); 
		        	  System.out.println("**********Search Results**********"); 
		        	  System.out.println(contactManager.find(new Contact(aname)));
		        	  System.out.println("**********************************");
		        	  break; 
		          case 4:
		        	  contactManager.sort();
		        	  break;
		          case 5:
		        	  exit = true;
		        	  System.out.println("Exiting Program... Goodbye.");
		        	  break;
		          default:
		        	  break;
		          }
		     } catch(Exception e) {
		    	 keyboard.nextLine();
		    	 System.out.println("Incorrect input, Try again..\n");
		     }
	     } while(!exit);
	}
}