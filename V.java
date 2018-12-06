import java.io.*;
import java.util.ArrayList;

public class V {
	
	int ID;                 	// Only used for sink nodes. Top sink node = 0, Bottom sink node = -1
	String operation;			// Operation of the vertex (+, *, /, etc.)
	String varName;				// The variable being operated on. ie: h = a * b... h is the varName
	ArrayList<V> successors;				// Array of successors
	int delay;					// Cycle delay. Multipliers = 2, divider/modulo = 3, all others = 1
	boolean isScheduled;		// True if scheduled, false otherwise
	int timing;					// Timing for ALAP
	int initialTime;			// Initial time from CDFG
	int final_time;				// Timing for list_r
	
	ArrayList<V> predecessors;
	
	// Constructor
	public V(int id, String operation, String varName) {
		this.ID = id;
		this.operation = operation;
		this.varName = varName;
		successors = new ArrayList<V>();
		predecessors = new ArrayList<V>();
		delay = calculateDelay(operation);
		isScheduled = false;
		timing = -1;
		initialTime = 1;
		final_time = -1;
	}
	
	// Sets the delay
	public void setDelay(int delay) {
		this.delay = delay;
	}
	
	public void setFinalTime(int time) {
		this.final_time = time;
	}
	
	public int getFinalTime() {
		return this.final_time;
	}
	
	public String getOperation() {
		return this.operation;
	}
	
	// Gets delay
	public int getDelay() {
		return this.delay;
	}
	
	// Adds a successor
	public void addSuccessor(V vertex) {
		this.successors.add(vertex);
	}
	
	// Returns successor array
	public ArrayList<V> getSuccessors() {
		return this.successors;
	}
	
	// Adds a successor
	public void addPredecessors(V vertex) {
		this.predecessors.add(vertex);
	}
	
	// Returns successor array
	public ArrayList<V> getPredecessors() {
		return this.predecessors;
	}
	
	public void setScheduled(boolean set) {
		this.isScheduled = set;
	}
	
	// Returns size of successor array
	public int getSuccessorSize() {
		return this.successors.size();
	}

	public void setTiming(int time) {
		this.timing = time;
	}

	public int getTiming() {
		return this.timing;
	}

	public int getID() {
		return this.ID;
	}
	
	public String getName() {
		return this.varName;
	}
	
	public boolean getIsScheduled() {
		return this.isScheduled;
	}
	
	public void setInitialTime(int time) {
		this.initialTime = time;
	}
	
	public int getInitialTime() {
		return this.initialTime;
	}
	/* 
   	 * Sets the delay for specified operator
	 * Multipliers = 2
	 * Divider/modulo = 3
	 * All others = 1
  	*/
	public int calculateDelay(String operator) {
		if(operator == "*") {
			return 2;
		}
		else if(operator == "/" || operator == "%") {
			return 3;
		}
		else {
			return 1;
		}
	}
}
