#ifndef _voxelquest_gameai_h_
#define _voxelquest_gameai_h_


class VNode
{
public:

    int tokenIndex;
    int ruleNumber;
    std::vector<VNode*> children;

    VNode(int _tokenIndex);

    ~VNode();

};

struct AssignStruct
{
    VNode* lastAssign;
    VNode* newAssign;
    int tokenIndex;
    int genIndex;
};

class GameAI
{
public:
    GameAI();

    void init(Singleton* _singleton);


    string getResolvedString(VNode* tempNode);

    bool setEqualTo(VNode* destNodeFinal, VNode* srceNodeFinal, int curGen);

    void rollBack(int curGen);

    bool attemptUnify(VNode* nodeToUnify, VNode* testNode, int curGen);

    VNode* drillDown(VNode* curVar);

    bool testEqual(VNode* testNode);

    bool isBetween(VNode* testNode);

    bool searchToUnify(VNode* nodeToUnifyBase, int curGen, int curOffset=0);


    void printChain(VNode* curVar);

    void runQueries();


    bool compileVocab(JSONValue* uncompiledNode, VNode* curVNode);


    bool checkVocab(JSONValue* jv);


    void buildRule(VNode* curNode, int ruleNumber, bool isIfThen);

    void buildRuleSet(VNode* curNode);




    void numberRule(VNode* curNode, int ruleNumber);

    int numberRuleSet(VNode* curNode, int offset);




    void printRule(VNode* curNode, int offset);

    void printRuleSet(VNode* curNode);



    bool parseKB();

    void traceRules();

    void getKB();

private:
    Singleton* singleton;

    std::vector<intVec> tokenToRules;
    std::vector<VToken> intToVToken;
    std::vector<VNode*> tokenIndexToVar;
    //std::vector<VNode*> goalList;
    std::vector<AssignStruct> assignStack;
    map<string, int> stringToVTokenIndex;

    string tempVarStr;

    VNode* kbCompiled;
    VNode* qbCompiled;


    JSONValue* jvKB;



};

// emotions:
    // ---------------
    // 


    // physical:
    // ---------------
    // pickup
    // drop
    // take from
    // place inside
    // move
    // toggle
    // kiss on cheek
    // kiss on mouth
    // hug
    // grope
    // reproduce (?)
    // slap
    // detain
    // 

    // verbal:
    // ----------------
    // threaten
    // insult
    // compliment
    // seduce
    // ask about (?)
    // 







    /*

    This comment is pulled from: http://www.learnprolognow.org/

    1) If term1 and term2 are constants, then term1 and term2 unify if and only
       if they are the same atom, or the same number.

    2) If term1 is a variable and term2 is any type of term, then term1 and term2 unify,
       and term1 is instantiated to term2 . Similarly, if term2 is a variable and term1 is
       any type of term, then term1 and term2 unify, and term2 is instantiated to term1.
       (So if they are both variables, they’re both instantiated to each other, and we say that they share values.)

    3) If term1 and term2 are complex terms, then they unify if and only if:
            - They have the same functor and arity, and <-- THIS IS ASSUMED TO BE THE CASE
            - all their corresponding arguments unify, and
            - the variable instantiations are compatible.
            (For example, it is not possible to instantiate variable X to mia when unifying one pair of arguments,
            and to instantiate X to vincent when unifying another pair of arguments.)

    Two terms unify if and only if it follows from the previous three clauses that they unify.

    First seek out if there is a rule that directly unifies. If not, find the head of a rule that matches.

    {k(X)} is true if {f(X),g(X),h(X) are all true}
    k(Y)?

    */


    /*



            node to unify:
            "pIsEquipable",["vX"]



            "oIfThen",[
              "pIsWorn",["vX"],
              "pIsEquipable",["vX"]
            ],

            "oIfThen",[
              "pIsHeld",["vX"],
              "pIsEquipable",["vX"]
            ],
                    "oIfThen",[
                      "pIsWeapon",["vX"],
                      "pIsHeld",["vX"]
                    ],

                            "oIfThen",[
                              "pIsSword",["vX"],
                              "pIsWeapon",["vX"]
                            ],
                                    "oIfThen",[
                                      "pIsDoombringer",["vX"],
                                      "oList",[
                                        "pIsSword",["vX"],
                                        "pIsSteel",["vX"],
                                        "pIsArtifact",["vX"]
                                      ]
                                    ],

                                    "oIfThen",[
                                      "pIsIronSword",["vX"],  <----
                                      "oList",[
                                        "pIsSword",["vX"],
                                        "pIsIron",["vX"]
                                      ]
                                    ],

                                    "pIsIronSword",["nItem0"]





    */

#endif//_voxelquest__h_