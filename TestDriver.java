import java.io.*;
import java.util.ArrayList;

public class TestDriver {
	
	public static void main(String args[]) {
		
		list_r test = new list_r();
		ArrayList<ArrayList<String>> temp = new ArrayList<>();
		ArrayList<String> arr1 = new ArrayList<String>();
		ArrayList<String> arr2 = new ArrayList<String>();
		ArrayList<String> arr3 = new ArrayList<String>();
		ArrayList<String> arr4 = new ArrayList<String>();
		ArrayList<String> arr5 = new ArrayList<String>();
		
//		arr1.add("f");
//		arr1.add("a");
//		arr1.add("*");
//		arr1.add("b");
//		
//		arr2.add("g");
//		arr2.add("f");
//		arr2.add("*");
//		arr2.add("c");
//		
//		arr3.add("h");
//		arr3.add("g");
//		arr3.add("*");
//		arr3.add("d");
//		
//		arr4.add("i");
//		arr4.add("h");
//		arr4.add("*");
//		arr4.add("e");

		
		arr1.add("a");
		arr1.add("b");
		arr1.add("+");
		arr1.add("c");
		
		arr2.add("d");
		arr2.add("e");
		arr2.add("*");
		arr2.add("f");
		
		arr3.add("h");
		arr3.add("a");
		arr3.add("+");
		arr3.add("d");
		
		arr4.add("x");
		arr4.add("h");
		arr4.add("+");
		arr4.add("c");
		
		arr5.add("g");
		arr5.add("i");
		arr5.add("+");
		arr5.add("m");
		
		temp.add(arr1);
		temp.add(arr2);
		temp.add(arr3);
		temp.add(arr4);
		temp.add(arr5);
		
		test.addVertices(temp, 0);

		test.addDependencies(temp, 0);
		test.alap(9);
		test.printInitialTimes();
		test.print_alap();
	}

}
