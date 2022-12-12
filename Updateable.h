#ifndef UPDATEABLE_H
#define UPDATEABLE_H

// Interface para auxiliar a atualizacao de varios objetos
class Updateable {
  public:
   
   virtual ~Updateable() {}
   virtual void update(double t) = 0;
};

#endif
