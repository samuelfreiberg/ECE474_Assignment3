import java.io.*;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class list_r {
	private ArrayList<V> vertices;
	private int v_id;
	
	public list_r() {
		vertices = new ArrayList<V>();
		
		vertices.add(new V(0, null, null) );	// Top Sink node
		vertices.add(new V(-1, null, null) );	// Bottom Sink node
		
		v_id = 1;
	}
	
	// Adds all vertices for specified section.
	// This gets called after all the declarations have been instantiated in the verilog file.
	// Variable "x" is the first line of statements in the input file'
	public void addVertices(ArrayList<ArrayList<String>> lines, int x) {
		
		// Adds new vertex for each statement
		for(int i = x; i < lines.size(); i++) {
			//ArrayList<String> line = tokenize(lines[i], " ");
			ArrayList<String> line = lines.get(i);
			// CHANGE BACK TO LINE
			String operator = line.get(line.size() - 2);
			vertices.add(new V(v_id, operator, line.get(0)) );	// Adds new vertex
			v_id++;
		}
	}
	
	// Searches for dependencies within the statements
	public void addDependencies(ArrayList<ArrayList<String>> lines, int x) {
		
		Set<String> set = new HashSet<String>();		// Set to hold variables already worked on (dependencies)
		
		for(int i = x; i < lines.size(); i++ ) {
			//ArrayList<String> line = tokenize(lines.get[i], " ");
			ArrayList<String> line = lines.get(i);
			
			// HAVE NOT ACCOUNTED FOR IF STATEMENTS YET
			// Determine if case: o = sel ? i1 : i0... I have not accounted for this case yet
			if(line.get(line.size()-2) != ":") {
				String var0 = line.get(0);
				String var1 = line.get(1);
				String var2 = line.get(line.size()-1);
				
				// If no dependency, add edge from sink node
				if(!set.contains(var1) && !set.contains(var2)) {	
					vertices.get(0).addSuccessor(findV(var0));			// Adds successor to sink node
					set.add(var0);
				}
				/* Initial Time is time from CDFG. Needed to compute slack for List_R scheduling */
				else {
					if(set.contains(var1) && set.contains(var2)) {
						findV(var1).addSuccessor(findV(var0));		// Adds successor
						findV(var2).addSuccessor(findV(var0));		// Adds successor
						findV(var0).setInitialTime(Math.max(findV(var1).getInitialTime() + findV(var1).getDelay(), findV(var2).getInitialTime() + findV(var2).getDelay()));
					}
					else if(set.contains(var1)) {
						findV(var1).addSuccessor(findV(var0));		// Adds successor
						findV(var0).setInitialTime(findV(var1).getInitialTime() + findV(var1).getDelay());
					}
					else if(set.contains(var2)) {
						findV(var2).addSuccessor(findV(var0));		// Adds successor
						findV(var0).setInitialTime(findV(var2).getInitialTime() + findV(var2).getDelay());
					}
					set.add(var0);
					vertices.get(0).addSuccessor(findV(var0));			// Adds successor to sink node
				}
			}
		}
		
		// Adds dependencies from leaf to bottom Sink node
		for(int j = 0; j < vertices.size(); j++) {
			if(vertices.get(j).getSuccessorSize() == 0) {	// If leaf, add successor to bottom sink node
				vertices.get(j).addSuccessor(vertices.get(1));
			}
		}
	}
	
	// Performs ALAP scheduling
	public void alap(int latency) {
		
		// Schedules the bottom sink node for upper latency bound + 1 and sets isScheduled to true
		vertices.get(1).setTiming(latency + 1);
		vertices.get(1).setScheduled(true);
		
		// while all vertices are not scheduled, run the algorithm
		while(!allVerticesScheduled()) {
					// Iterate through vertices, find a vertex where all successors are scheduled
					for(int i = 0; i < vertices.size(); i++) {	
					
						V currentV = vertices.get(i);
						boolean valid = true;		// boolean to check if all successors are scheduled
						int lowestTime = 100;		// For multiple successors, holds minimum time
						 
						if(currentV.getIsScheduled() == false) {			// If vertex is not scheduled, check all it's successors to determine if their scheduled
							ArrayList<V> successors = currentV.getSuccessors();
							for(int j = 0; j < successors.size(); j++) {
								if(successors.get(j).getIsScheduled() == false) {		// If all successors are not scheduled, break
									valid = false;
									break;
								}
								else {
									if(successors.get(j).getTiming() < lowestTime) {
										lowestTime = successors.get(j).getTiming();
									}
								}
							}	
							if(valid == true) {
								if(lowestTime-vertices.get(i).getDelay() < 1 && i != 0) {
									System.out.println("Latency not possible!");
									return;
								}
								vertices.get(i).setTiming(lowestTime - vertices.get(i).getDelay());
								vertices.get(i).setScheduled(true);
							}
						}
					}
		}	
		// Top sink node is always time 0
		vertices.get(0).setTiming(0);
	}
	
	/* Runs list_r scheduling */
	public void listR_Scheduling() {
		
	}
	
	
	// Finds vertex by name
	public V findV(String varName) {
		for(int i = 0; i < vertices.size(); i++) {
			if(vertices.get(i).getID() != 0 && vertices.get(i).getID() != -1 && vertices.get(i).varName.equals(varName)) {
				return vertices.get(i);
			}
		}
		return null;
	}

	
	// Returns true if all vertices scheduled, false otherwise
	public boolean allVerticesScheduled() {
		for(int i = 0; i < vertices.size(); i++) {
			if(vertices.get(i).getIsScheduled() == false) {
				return false;
			}
		}
		return true;
	}
	// Breaks up statement into array of words
	public ArrayList<String> tokenize(String s, String tokenizer) {
	    int pos;
	    ArrayList<String> result = new ArrayList<String>();
		
	    while ( (pos = s.indexOf(tokenizer) ) != s.length()) {
	        result.add(s.substring(0, pos));
	        String temp = s.substring(0, pos + tokenizer.length());
	        s = s.replace(temp, "");
	    }
	    if (s != " ") {
	        result.add(s);
	    }
	    return result;
	}
	
	public void printV() {
		for(int i = 0; i < vertices.size(); i++) {
			System.out.println("Vertex #" + i + ": " + vertices.get(i).getName() + ", " + vertices.get(i).getOperation());
		}
	}
	
	public void printDependencies() {
		for(int i = 0; i < vertices.size(); i++) {
			System.out.print("Vertex " + vertices.get(i).getName() + " has the following dependencies: ");
			
			for(int j = 0; j < vertices.get(i).getSuccessorSize(); j++) {
				System.out.print(vertices.get(i).getSuccessors().get(j).getName() + ", ");
			}
			System.out.println();
		}
	}
	
	public void print_alap() {
		for(int i = 0; i < vertices.size(); i++) {
			System.out.println("Vertex " + vertices.get(i).getName() + " is scheduled in time " + vertices.get(i).getTiming());
		}
	}
	public void printDelays() {
		for(int i = 0; i < vertices.size(); i++) {
			System.out.println(vertices.get(i).getDelay());
		}
	}
	
	public void printInitialTimes() {
		for(int i = 2; i < vertices.size(); i++) {
			System.out.println(vertices.get(i).initialTime);
		}
	}
}
