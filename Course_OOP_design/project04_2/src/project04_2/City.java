package project04_2;

public class City {
	
	private String name;
	private double lat;
	private double lon;
	
	public City() {}
	
	public City(String name,double lat, double lon) {
		/*your code*/
		this.name = name;
		this.lat = lat;
		this.lon = lon;
		
	}
	
	public City(String name) {
		this.name = name;
		this.lat = (int)(Math.random()*360);
		this.lon = (int)(Math.random()*360);
	}
	
	public boolean equals(City otherObject) {
		/*your code*/
		if( this.name.equals(otherObject.name) 
				&& this.lat == otherObject.lat
				&& this.lon == otherObject.lon)
			return true;
		else
			return false;
	}
	
	public String toString() {
		/*your code*/
		return (this.name + " " + this.lat + " " + this.lon);
	}
	
	public static double cityDistance(City otherObject1,City otherObject2) {
		/*your code*/
		double lon = otherObject1.lon - otherObject2.lon;
		double lat = otherObject1.lat - otherObject2.lat;
		return Math.sqrt(lon*lon + lat*lat );
	}
}
