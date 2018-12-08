else {
				string var0 = line[0]; // z = g ? d : e   ----> z
				string var1 = line[2];// g
				string var2 = line[4];// d
				string var3 = line[6];// e

				const bool is_in1 = set1.find(var1) != set1.end();
				const bool is_in2 = set1.find(var2) != set1.end();
				const bool is_in3 = set1.find(var3) != set1.end();

				// If no dependency, add edge from sink node
				if (is_in1 == false && is_in2 == false && is_in3 == false) {
					//vertices.at(0).addSuccessor(findV(var0));            // Adds successor to sink node
					vertices.at(0).addSuccessor(vertices.at(getIndex(var0)));
					//findV(var0).addPredecessors(vertices.at(0));        // Adds predecessor
					vertices.at(getIndex(var0)).addPredecessors(vertices.at(0));
					//cout << "Neither" << endl;
					set1.insert(var0);
				}
				/* Initial Time is time from CDFG. Needed to compute slack for List_R scheduling */
				else {
					if (is_in1 == true && is_in2 == true && is_in3 == true) {
						//cout << "Both" << endl;

						vertices.at(getIndex(var1)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var2)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var3)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var1)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var2)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var3)));
						vertices.at(getIndex(var0)).setInitialTime(max(max(vertices.at(getIndex(var1)).getInitialTime() + vertices.at(getIndex(var1)).getDelay(), vertices.at(getIndex(var2)).getInitialTime() + vertices.at(getIndex(var2)).getDelay()), vertices.at(getIndex(var3)).getInitialTime() + vertices.at(getIndex(var3)).getDelay()));

						//findV(var1).addSuccessor(findV(var0));        // Adds successor
						//findV(var2).addSuccessor(findV(var0));        // Adds successor
						//findV(var0).addPredecessors(findV(var1));    // Adds predecessor
						//findV(var0).addPredecessors(findV(var2));    // Adds predecessor
						//findV(var0).setInitialTime(max(findV(var1).getInitialTime() + findV(var1).getDelay(), findV(var2).getInitialTime() + findV(var2).getDelay()));
					}
					else if (is_in1 == true && is_in2 == true && is_in3 == false) {
						//cout << var1 << " is dependent" << endl;

						vertices.at(getIndex(var1)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var2)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var1)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var2)));
						vertices.at(getIndex(var0)).setInitialTime(max(vertices.at(getIndex(var1)).getInitialTime() + vertices.at(getIndex(var1)).getDelay(), vertices.at(getIndex(var2)).getInitialTime() + vertices.at(getIndex(var2)).getDelay()));
					}
					else if (is_in1 == false && is_in2 == true && is_in3 == true) {
						//cout << var2 << " is dependent" << endl;
						//findV(var2).addSuccessor(findV(var0));        // Adds successor
						//findV(var0).addPredecessors(findV(var2));    // Adds predecessor
						//findV(var0).setInitialTime(findV(var2).getInitialTime() + findV(var2).getDelay());

						vertices.at(getIndex(var2)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var3)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var2)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var3)));
						vertices.at(getIndex(var0)).setInitialTime(max(vertices.at(getIndex(var2)).getInitialTime() + vertices.at(getIndex(var2)).getDelay(), vertices.at(getIndex(var3)).getInitialTime() + vertices.at(getIndex(var3)).getDelay()));
						
					}
					else if (is_in1 == true && is_in2 == false && is_in3 == true) {
						//cout << var2 << " is dependent" << endl;
						//findV(var2).addSuccessor(findV(var0));        // Adds successor
						//findV(var0).addPredecessors(findV(var2));    // Adds predecessor
						//findV(var0).setInitialTime(findV(var2).getInitialTime() + findV(var2).getDelay());

						vertices.at(getIndex(var1)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var3)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var1)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var3)));
						vertices.at(getIndex(var0)).setInitialTime(max(vertices.at(getIndex(var1)).getInitialTime() + vertices.at(getIndex(var1)).getDelay(), vertices.at(getIndex(var3)).getInitialTime() + vertices.at(getIndex(var3)).getDelay()));

					}
					else if (is_in1 == true && is_in2 == false && is_in3 == false) {
						//cout << var2 << " is dependent" << endl;
						//findV(var2).addSuccessor(findV(var0));        // Adds successor
						//findV(var0).addPredecessors(findV(var2));    // Adds predecessor
						//findV(var0).setInitialTime(findV(var2).getInitialTime() + findV(var2).getDelay());

						vertices.at(getIndex(var1)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var1)));
						vertices.at(getIndex(var0)).setInitialTime(vertices.at(getIndex(var1)).getInitialTime() + vertices.at(getIndex(var1)).getDelay());
					}
					else if (is_in1 == false && is_in2 == true && is_in3 == false) {
						//cout << var2 << " is dependent" << endl;
						//findV(var2).addSuccessor(findV(var0));        // Adds successor
						//findV(var0).addPredecessors(findV(var2));    // Adds predecessor
						//findV(var0).setInitialTime(findV(var2).getInitialTime() + findV(var2).getDelay());

						vertices.at(getIndex(var2)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var2)));
						vertices.at(getIndex(var0)).setInitialTime(vertices.at(getIndex(var2)).getInitialTime() + vertices.at(getIndex(var2)).getDelay());
					}
					else if (is_in1 == false && is_in2 == false && is_in3 == true) {
						//cout << var2 << " is dependent" << endl;
						//findV(var2).addSuccessor(findV(var0));        // Adds successor
						//findV(var0).addPredecessors(findV(var2));    // Adds predecessor
						//findV(var0).setInitialTime(findV(var2).getInitialTime() + findV(var2).getDelay());

						vertices.at(getIndex(var3)).addSuccessor(vertices.at(getIndex(var0)));
						vertices.at(getIndex(var0)).addPredecessors(vertices.at(getIndex(var3)));
						vertices.at(getIndex(var0)).setInitialTime(vertices.at(getIndex(var3)).getInitialTime() + vertices.at(getIndex(var3)).getDelay());
					}
					//cout << "Inserting " << var0 << endl;
					set1.insert(var0);
					vertices.at(0).addSuccessor(vertices.at(getIndex(var0)));            // Adds successor to sink node
				}

			}
