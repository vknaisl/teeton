#include<iostream>
#include <unordered_map>

using namespace std;

void runtimeError(string err) {
  cout << "RuntimeError: " << err << endl;
  exit(1);
}

enum ArithmeticOperator { ADD, SUB, MUL, DIV, MOD };

enum EqualOperator { EQ, NEQ };

enum OrderOperator { GT, LT, GTE, LTE };

enum BoolOperator { AND, OR };

enum Type { CHAR, BOOL, INT };


class AbstractType {
public:
  virtual Type type() = 0;
  virtual bool equals(EqualOperator op, AbstractType * other);
  virtual bool ord(OrderOperator op, AbstractType * other);
};

bool AbstractType::equals(EqualOperator op, AbstractType * other) {
  runtimeError("Equals not implemented for given type");
  return false;
}

bool AbstractType::ord(OrderOperator op, AbstractType * other) {
  runtimeError("Ord not implemented for given type");
  return false;
}


bool isEqualable(AbstractType * type) {
  if (type->type() == INT || type->type() == CHAR) {
    return true;
  }
  return false;
}


bool isOrderable(AbstractType * type) {
  if (type->type() == INT || type->type() == CHAR) {
    return true;
  }
  return false;
}


class TypeChar : public AbstractType {
public:
  TypeChar(char value) : _value(value) {};
  virtual bool equals(EqualOperator op, AbstractType * other);
  virtual bool ord(OrderOperator ord, AbstractType * other);
  virtual Type type();
  char value();
private:
  char _value;
};

Type TypeChar::type() {
  return CHAR;
}

char TypeChar::value() {
  return _value;
}

bool TypeChar::equals(EqualOperator op, AbstractType * other) {
  TypeChar * otherChar = (TypeChar *) other;
  switch(op) {
    case EQ:
      return _value == otherChar->value();
    case NEQ:
      return _value != otherChar->value();
  }
  return false;
}

bool TypeChar::ord(OrderOperator op, AbstractType * other) {
  TypeChar * otherChar = (TypeChar *) other;
  switch(op) {
    case GT:
      return _value > otherChar->value();
    case LT:
      return _value < otherChar->value();
    case GTE:
      return _value >= otherChar->value();
    case LTE:
      return _value <= otherChar->value();
  }
  return false;
}


class TypeBool : public AbstractType {
public:
  TypeBool(bool value) : _value(value) {};
  virtual bool equals(EqualOperator op, AbstractType * other);
  virtual Type type();
  bool value();
private:
  bool _value;
};

Type TypeBool::type() {
  return BOOL;
}

bool TypeBool::value() {
  return _value;
}

bool TypeBool::equals(EqualOperator op, AbstractType * other) {
  TypeBool * otherBool = (TypeBool *) other;
  switch(op) {
    case EQ:
      return _value == otherBool->value();
    case NEQ:
      return _value != otherBool->value();
  }
  return false;
}


class TypeInt : public AbstractType {
public:
  TypeInt(int value) : _value(value) {};
  virtual Type type();
  virtual bool equals(EqualOperator op, AbstractType * other);
  virtual bool ord(OrderOperator ord, AbstractType * other);
  int value();
private:
  int _value;
};

Type TypeInt::type() {
  return INT;
}

int TypeInt::value() {
  return _value;
}

bool TypeInt::equals(EqualOperator op, AbstractType * other) {
  TypeInt * otherInt = (TypeInt *) other;
  switch(op) {
    case EQ:
      return _value == otherInt->value();
    case NEQ:
      return _value != otherInt->value();
  }
  return false;
}

bool TypeInt::ord(OrderOperator op, AbstractType * other) {
  TypeInt * otherInt = (TypeInt *) other;
  switch(op) {
    case GT:
      return _value > otherInt->value();
    case LT:
      return _value < otherInt->value();
    case GTE:
      return _value >= otherInt->value();
    case LTE:
      return _value <= otherInt->value();
  }
  return false;
}


// global variable storage
unordered_map<string, AbstractType *> variables;


class AbstractNode {
public:
  virtual AbstractType * evaluate() = 0;
  virtual ~AbstractNode() = 0;
};

inline AbstractNode::~AbstractNode() { }


class NodeBlock : public AbstractNode {
public:
  NodeBlock(AbstractNode ** nodes, int nodes_count) : nodes(nodes), nodes_count(nodes_count) {};
  ~NodeBlock();
  virtual AbstractType * evaluate();
private:
  AbstractNode ** nodes;
  int nodes_count;
};

AbstractType * NodeBlock::evaluate() {
  for (int i = 0; i < nodes_count; i++) {
    nodes[i]->evaluate();
  }
  return NULL;
}

NodeBlock::~NodeBlock() {
  for (int i = 0; i < nodes_count; i++) {
    delete nodes[i];
  }
}


class NodeVariableDefinition : public AbstractNode {
public:
  NodeVariableDefinition(string name, AbstractNode * value) : name(name), value(value) {};
  ~NodeVariableDefinition();
  virtual AbstractType * evaluate();
private:
  string name;
  AbstractNode * value;
};

AbstractType * NodeVariableDefinition::evaluate() {
  variables[name] = value->evaluate();
  return NULL;
}

NodeVariableDefinition::~NodeVariableDefinition() {
  delete value;
}


class NodeVariableName : public AbstractNode {
public:
  NodeVariableName(string name) : name(name) {};
  virtual AbstractType * evaluate();
private:
  string name;
};

AbstractType * NodeVariableName::evaluate() {
  return variables[name];
}


class NodePrint : public AbstractNode {
public:
  NodePrint(AbstractNode * value) : value(value) {};
  ~NodePrint();
  virtual AbstractType * evaluate();
private:
  AbstractNode * value;
};

AbstractType * NodePrint::evaluate() {
  AbstractType * evaluated = value->evaluate();

  switch(evaluated->type()) {
    case BOOL:
      cout << (((TypeBool *) evaluated)->value() ? "True" : "False") << endl;
      break;
    case CHAR:
      cout << ((TypeChar *) evaluated)->value() << endl;
      break;
    case INT:
      cout << ((TypeInt *) evaluated)->value() << endl;
      break;
  }

  return NULL;
}

NodePrint::~NodePrint() {
  delete value;
}


class NodeArithmeticOperator : public AbstractNode {
public:
  NodeArithmeticOperator(ArithmeticOperator op, AbstractNode * a, AbstractNode * b) : op(op), a(a), b(b) {};
  ~NodeArithmeticOperator();
  virtual AbstractType * evaluate();
private:
  ArithmeticOperator op;
  AbstractNode * a;
  AbstractNode * b;
};

AbstractType * NodeArithmeticOperator::evaluate() {
  AbstractType * t1 = a->evaluate();
  AbstractType * t2 = b->evaluate();

  if (t1->type() != INT || t2->type() != INT) {
    runtimeError("Using arithmetic operation with non-integer variable");
  }

  TypeInt * i1 = (TypeInt *) t1;
  TypeInt * i2 = (TypeInt *) t2;

  switch (op) {
    case ADD:
      return new TypeInt(i1->value() + i2->value());
    case SUB:
      return new TypeInt(i1->value() - i2->value());
    case MUL:
      return new TypeInt(i1->value() * i2->value());
    case DIV:
      return new TypeInt(i1->value() / i2->value());
    case MOD:
      return new TypeInt(i1->value() % i2->value());
  }
  return NULL;
}

NodeArithmeticOperator::~NodeArithmeticOperator() {
  delete a;
  delete b;
}

class NodeEqualOperator : public AbstractNode {
public:
  NodeEqualOperator(EqualOperator op, AbstractNode * a, AbstractNode * b) : op(op), a(a), b(b) {};
  ~NodeEqualOperator();
  virtual AbstractType * evaluate();
private:
  EqualOperator op;
  AbstractNode * a;
  AbstractNode * b;
};

AbstractType * NodeEqualOperator::evaluate() {
  AbstractType * t1 = a->evaluate();
  AbstractType * t2 = b->evaluate();

  if (!isEqualable(t1) || !isEqualable(t2)) {
    runtimeError("Types are not equalable");
  }

  if (t1->type() != t2->type()) {
    return new TypeBool(false);
  }

  return new TypeBool(t1->equals(op, t2));
}

NodeEqualOperator::~NodeEqualOperator() {
  delete a;
  delete b;
}


class NodeOrderOperator : public AbstractNode {
public:
  NodeOrderOperator(OrderOperator op, AbstractNode * a, AbstractNode * b) : op(op), a(a), b(b) {};
  ~NodeOrderOperator();
  virtual AbstractType * evaluate();
private:
  OrderOperator op;
  AbstractNode * a;
  AbstractNode * b;
};

AbstractType * NodeOrderOperator::evaluate() {
  AbstractType * t1 = a->evaluate();
  AbstractType * t2 = b->evaluate();

  if (!isOrderable(t1) || !isOrderable(t2)) {
    runtimeError("Types are not orderable");
  }

  if (t1->type() != t2->type()) {
    runtimeError("Order operator can be used only for same types");
  }

  return new TypeBool(t1->ord(op, t2));
}

NodeOrderOperator::~NodeOrderOperator() {
  delete a;
  delete b;
}


class NodeBooleanOperator : public AbstractNode {
public:
  NodeBooleanOperator(BoolOperator op, AbstractNode * a, AbstractNode * b) : op(op), a(a), b(b) {};
  ~NodeBooleanOperator();
  virtual AbstractType * evaluate();
private:
  BoolOperator op;
  AbstractNode * a;
  AbstractNode * b;
};

AbstractType * NodeBooleanOperator::evaluate() {
  AbstractType * t1 = a->evaluate();
  AbstractType * t2 = b->evaluate();

  if (t1->type() != BOOL || t2->type() != BOOL) {
    runtimeError("Using boolean operation with non-boolean variable");
  }

  TypeBool * b1 = (TypeBool *) t1;
  TypeBool * b2 = (TypeBool *) t2;

  switch (op) {
    case AND:
      return new TypeBool(b1->value() && b2->value());
    case OR:
      return new TypeBool(b1->value() || b2->value());
  }
  return new TypeBool(false);
}

NodeBooleanOperator::~NodeBooleanOperator() {
  delete a;
  delete b;
}


class NodeNotOperator : public AbstractNode {
public:
  NodeNotOperator(AbstractNode * a) : a(a) {};
  ~NodeNotOperator();
  virtual AbstractType * evaluate();
private:
  AbstractNode * a;
};

AbstractType * NodeNotOperator::evaluate() {
  AbstractType * t = a->evaluate();

  if (t->type() != BOOL) {
    runtimeError("Using not operator with non-boolean variable");
  }

  TypeBool * b = (TypeBool *) t;
  return new TypeBool(!b->value());
}

NodeNotOperator::~NodeNotOperator() {
  delete a;
}


class NodeConstant : public AbstractNode {
public:
  NodeConstant(AbstractType * value) : value(value) {};
  virtual AbstractType * evaluate();
private:
  AbstractType * value;
};

AbstractType * NodeConstant::evaluate() {
  return value;
}


class NodeWhile : public AbstractNode {
public:
  NodeWhile(AbstractNode * condition, NodeBlock * block) : condition(condition), block(block) {};
  ~NodeWhile();
  virtual AbstractType * evaluate();
private:
  AbstractNode * condition;
  NodeBlock * block;
};

AbstractType * NodeWhile::evaluate() {
  for(;;) {
    AbstractType * evaluated = condition->evaluate();

    if (evaluated->type() != BOOL) {
      runtimeError("Cannot use non-bool value for condition");
    }

    TypeBool * evaluatedBool = (TypeBool *) evaluated;

    if (!evaluatedBool->value()) {
      return NULL;
    }

    block->evaluate();
  }
  return NULL;
}

NodeWhile::~NodeWhile() {
  delete condition;
  delete block;
}


class NodeIfElse : public AbstractNode {
public:
  NodeIfElse(AbstractNode * condition, NodeBlock * ifBlock, NodeBlock * elseBlock) : condition(condition), ifBlock(ifBlock), elseBlock(elseBlock) {};
  ~NodeIfElse();
  virtual AbstractType * evaluate();
private:
  AbstractNode * condition;
  NodeBlock * ifBlock;
  NodeBlock * elseBlock;
};

AbstractType * NodeIfElse::evaluate() {
  AbstractType * evaluated = condition->evaluate();

  if (evaluated->type() != BOOL) {
    runtimeError("Cannot use non-bool value for condition");
  }

  TypeBool * evaluatedBool = (TypeBool *) evaluated;

  if (evaluatedBool->value()) {
    ifBlock->evaluate();
  } else {
    elseBlock->evaluate();
  }

  return NULL;
}

NodeIfElse::~NodeIfElse() {
  delete ifBlock;
  delete elseBlock;
}



// EXAMPLES ---

/**
 * var a = 5
 * var b = 2
 * var c = a - b
 * print(c)
 */
void example_arithmetic() {
  cout << "* Example arithmetic. Expected result: 3" << endl;
  AbstractNode * nodes[4] = {
    new NodeVariableDefinition("a", new NodeConstant(new TypeInt(5))),
    new NodeVariableDefinition("b", new NodeConstant(new TypeInt(2))),
    new NodeVariableDefinition("c", new NodeArithmeticOperator(SUB, new NodeVariableName("a"), new NodeVariableName("b"))),
    new NodePrint(new NodeVariableName("c"))
  };
  NodeBlock * root = new NodeBlock(nodes, 4);
  root->evaluate();
  delete root;
  cout << endl;
}

/**
 * var a = 5
 * var b = 5
 * var c = a >= b
 * print(c)
 */
void example_comparison() {
  cout << "* Example comparison. Expected result: True" << endl;
  AbstractNode * nodes[4] = {
    new NodeVariableDefinition("a", new NodeConstant(new TypeInt(5))),
    new NodeVariableDefinition("b", new NodeConstant(new TypeInt(5))),
    new NodeVariableDefinition("c", new NodeOrderOperator(GTE, new NodeVariableName("a"), new NodeVariableName("b"))),
    new NodePrint(new NodeVariableName("c"))
  };
  NodeBlock * root = new NodeBlock(nodes, 4);
  root->evaluate();
  delete root;
  cout << endl;
}

/**
 * var i = 0
 * while(i < 10) {
 *   print(i)
 *   i = i + 1
 * }
 */
void example_whileCycle() {
  cout << "* Example while cycle. Expected result: lines 0 .. 9" << endl;
  AbstractNode * whileBlockNodes[2] = {
    new NodePrint(new NodeVariableName("i")),
    new NodeVariableDefinition("i", new NodeArithmeticOperator(ADD, new NodeVariableName("i"), new NodeConstant(new TypeInt(1))))
  };
  AbstractNode * nodes[2] = {
    new NodeVariableDefinition("i", new NodeConstant(new TypeInt(0))),
    new NodeWhile(
      new NodeOrderOperator(LT, new NodeVariableName("i"), new NodeConstant(new TypeInt(10))),
      new NodeBlock(whileBlockNodes, 2)
    )
  };
  NodeBlock * root = new NodeBlock(nodes, 2);
  root->evaluate();
  delete root;
  cout << endl;
}

/**
 * var i = 'x'
 * if (i == 'x') {
 *   print('a')
 * } else {
 *   print('n')
 * }
 */
void example_ifElse() {
  cout << "* Example if else. Expected result: 'a'" << endl;
  AbstractNode * ifBlockNodes[1] = { new NodePrint(new NodeConstant(new TypeChar('a'))) };
  AbstractNode * elseBlockNodes[1] = { new NodePrint(new NodeConstant(new TypeChar('n'))) };
  AbstractNode * nodes[2] = {
    new NodeVariableDefinition("i", new NodeConstant(new TypeChar('x'))),
    new NodeIfElse(
      new NodeEqualOperator(EQ, new NodeVariableName("i"), new NodeConstant(new TypeChar('x'))),
      new NodeBlock(ifBlockNodes, 1),
      new NodeBlock(elseBlockNodes, 1)
    )
  };
  NodeBlock * root = new NodeBlock(nodes, 2);
  root->evaluate();
  delete root;
  cout << endl;
}

/**
 * var i = 'x'
 * var x1 = 2
 * var x2 = 5
 * if (!((x1 < x2) && (i == 'x'))) {
 *   print('a')
 * } else {
 *   print('n')
 * }
 */
void example_complicatedCondition() {
  cout << "* Example complicated condition. Expected result: 'n'" << endl;
  AbstractNode * ifBlockNodes[1] = { new NodePrint(new NodeConstant(new TypeChar('a'))) };
  AbstractNode * elseBlockNodes[1] = { new NodePrint(new NodeConstant(new TypeChar('n'))) };
  AbstractNode * nodes[4] = {
    new NodeVariableDefinition("i", new NodeConstant(new TypeChar('x'))),
    new NodeVariableDefinition("x1", new NodeConstant(new TypeInt(2))),
    new NodeVariableDefinition("x2", new NodeConstant(new TypeInt(5))),
    new NodeIfElse(
      new NodeNotOperator(new NodeBooleanOperator(
        AND,
        new NodeOrderOperator(LT, new NodeVariableName("x1"), new NodeVariableName("x2")),
        new NodeEqualOperator(EQ, new NodeVariableName("i"), new NodeConstant(new TypeChar('x')))
      )),
      new NodeBlock(ifBlockNodes, 1),
      new NodeBlock(elseBlockNodes, 1)
    )
  };
  NodeBlock * root = new NodeBlock(nodes, 4);
  root->evaluate();
  delete root;
  cout << endl;
}

int main() {
  example_arithmetic();
  example_comparison();
  example_whileCycle();
  example_ifElse();
  example_complicatedCondition();
  return 0;
}
