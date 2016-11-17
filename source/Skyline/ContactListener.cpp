#include "ContactListener.h"
#include "Entity.h"
#include <iostream>

ContactListener::ContactListener() {}

ContactListener::~ContactListener() { }

void ContactListener::BeginContact(b2Contact* contact) {
  Entity* entityA  = static_cast<Entity*> (contact->GetFixtureA()->GetBody()->GetUserData());
  Entity* entityB = static_cast<Entity*> (contact->GetFixtureB()->GetBody()->GetUserData());

  if(entityA && entityB) {
    entityA->contact(entityB);
    entityB->contact(entityA);
  }
}

void ContactListener::EndContact(b2Contact* contact) {

}