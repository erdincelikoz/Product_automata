//
// Created by erdin on 4/27/2025.
//
#include <iostream>
#include "DFA.h"
#include "json.hpp"
#include <iomanip>
#include <string>
#include <fstream>
#include <queue>
#include <algorithm>
using namespace std;
using json = nlohmann::json;


DFA::DFA(string filename) {
    ifstream input(filename);
    json j = json::parse(input);

    setAlphabet(j["alphabet"]);

    vector<json> inputstates = j["states"];
    vector<State> parsedstates;
    vector<State> acceptstates;
    for (int i = 0; i < inputstates.size(); i++) {
        State temp;
        temp.name = inputstates[i]["name"];
        temp.accepting = inputstates[i]["accepting"];
        temp.starting = inputstates[i]["starting"];
        parsedstates.push_back(temp);
        if (inputstates[i]["accepting"] == true) {
            acceptstates.push_back(temp);
        }
        if (inputstates[i]["starting"] == true) {
            setStartstate(inputstates[i]);
        }
    }
    setStates(parsedstates);

    vector<json> inputtransitions = j["transitions"];
    vector<Transition> parsedtransitions;
    for (int i = 0; i < inputtransitions.size(); i++) {
        Transition temp;
        parsedtransitions.push_back(temp);
        parsedtransitions[i].from = inputtransitions[i]["from"];
        parsedtransitions[i].input = inputtransitions[i]["input"];
        parsedtransitions[i].to = inputtransitions[i]["to"];
    }
    setTransitions(parsedtransitions);
}




DFA::DFA(DFA dfa1, DFA dfa2, bool intersection_union) {
    setAlphabet(dfa1.getAlphabet());
    string productstartname = "(" + dfa1.getStartstate().name + "," + dfa2.getStartstate().name + ")";
    State productstartstate;
    productstartstate.name = productstartname;
    productstartstate.starting = true;
    bool temp1 = false;
    bool temp2 = false;
    for (int x=0; x < dfa1.getEndstates().size(); x++) {
        if (dfa1.getEndstates()[x].name == dfa1.getStartstate().name) {
            temp1 = true;
            break;
        }
    }
    for (int y=0; y < dfa2.getEndstates().size(); y++) {
        if (dfa2.getEndstates()[y].name == dfa2.getStartstate().name) {
            temp2 = true;
            break;
        }
    }
    if (intersection_union) {
        if (temp1 && temp2) {
            productstartstate.accepting = true;
        }
        else {
            productstartstate.accepting = false;
        }
    }
    else {
        if (temp1 || temp2) {
            productstartstate.accepting = true;
        }
        else {
            productstartstate.accepting = false;
        }
    }
    getStates().push_back(productstartstate);



    queue<pair<string,string>> tovisit;
    tovisit.push({dfa1.getStartstate().name,dfa2.getStartstate().name});
    vector<string> visited;
    visited.push_back(productstartstate.name);
    while (!tovisit.empty()) {
        pair<string,string> current = tovisit.front();
        tovisit.pop();

        string currentname = "(" + current.first + "," + current.second + ")";
        for (int i = 0; i < getAlphabet().size(); i++) {
            string firstnext = "";
            string secondnext = "";
            vector<Transition> transitions1 = dfa1.getTransitions();
            for (int j = 0; j<transitions1.size(); j++) {
                if (transitions1[j].from == current.first && transitions1[j].input == getAlphabet()[i]) {
                    firstnext = transitions1[j].to;
                    break;
                }
            }
            vector<Transition> transitions2 = dfa2.getTransitions();
            for (int k = 0; k<transitions2.size(); k++) {
                if (transitions2[k].from == current.second && transitions2[k].input == getAlphabet()[i]) {
                    secondnext = transitions2[k].to;
                    break;
                }
            }

            if (firstnext != "" && secondnext != "") {
                string nextproductname = "(" + firstnext + "," + secondnext + ")";

                if (find(visited.begin(), visited.end(), nextproductname) == visited.end()) {
                    State nextproductstate;
                    nextproductstate.name = nextproductname;
                    nextproductstate.starting = false;

                    bool temp1 = false;
                    bool temp2 = false;
                    for (int x=0; x < dfa1.getEndstates().size(); x++) {
                        if (dfa1.getEndstates()[x].name == firstnext) {
                            temp1 = true;
                            break;
                        }
                    }
                    for (int y=0; y < dfa2.getEndstates().size(); y++) {
                        if (dfa2.getEndstates()[y].name == secondnext) {
                            temp2 = true;
                            break;
                        }
                    }
                    if (intersection_union) {
                        if (temp1 && temp2) {
                            nextproductstate.accepting = true;
                            getEndstates().push_back(nextproductstate);
                        }
                        else {
                            nextproductstate.accepting = false;
                        }
                    }
                    else {
                        if (temp1 || temp2) {
                            nextproductstate.accepting = true;
                            getEndstates().push_back(nextproductstate);
                        }
                        else {
                            nextproductstate.accepting = false;
                        }
                    }
                    getStates().push_back(nextproductstate);
                    visited.push_back(nextproductname);
                    tovisit.push(pair(firstnext, secondnext));
                }
                Transition nexttransition;
                nexttransition.from = currentname;
                nexttransition.input = getAlphabet()[i];
                nexttransition.to = nextproductname;
                getTransitions().push_back(nexttransition);
            }

        }
    }
}

void DFA::print() {

    json j2;
    j2["type"]     = "DFA";
    j2["alphabet"] = getAlphabet();

    j2["states"] = json::array();
    for (State state : getStates()) {
        json s;
        s["name"]     = state.name;
        s["starting"] = state.starting;
        s["accepting"]= state.accepting;
        j2["states"].push_back(s);
    }

    j2["transitions"] = json::array();
    for (Transition transition : getTransitions()) {
        json t;
        t["from"]  = transition.from;
        t["to"]    = transition.to;
        t["input"] = transition.input;
        j2["transitions"].push_back(t);
    }

    cout << setw(4) << j2 << endl;
}