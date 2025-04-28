    #ifndef DFA_H
    #define DFA_H
    #include <string>
    #include <vector>
    #include "json.hpp"
    using json = nlohmann::json;
    using namespace std;

    struct State {
        string name;
        bool starting;
        bool accepting;
    };
    struct Transition {
        string from;
        string to;
        string input;
    };


    class DFA {
    public:
        DFA(string filename);
        DFA(DFA dfa1, DFA dfa2, bool intersection_union);




        void print();




        void setAlphabet(vector<string> newalphabet) {
            alphabet = newalphabet;
        }
        vector<string>& getAlphabet() {
            return alphabet;
        }




        void setStates(vector<State> newstates){
            states = newstates;
        }
        vector<State>& getStates() {
            return states;
        }




        void setStartstate(State newstartstate) {
            startstate = newstartstate;
        }
        void setStartstate(json jsonstate) {
            startstate.name = jsonstate["name"];
            startstate.accepting = jsonstate["accepting"];
            startstate.starting = jsonstate["starting"];
        }
        State getStartstate() {
            return startstate;
        }




        void setEndstates(vector<State> newendstates) {
            endstates = newendstates;
        }
        vector<State>& getEndstates() {
            return endstates;
        }




        void setTransitions(vector<Transition> newtransitions) {
            transitions = newtransitions;
        }

        vector<Transition>& getTransitions() {
            return transitions;
        }




    private:
        vector<string> alphabet;
        vector<State> states;
        State startstate;
        vector<State> endstates;
        vector<Transition> transitions;
    };



    #endif //DFA_H
