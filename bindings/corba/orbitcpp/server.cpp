/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include "swordorb-impl.hpp"
#include <iostream>
#include <swmgr.h>
#include "webmgr.hpp"

WebMgr swordmgr;



int main (int argc, char* argv[]) {
  try {
 	  // Initialize the CORBA orb
 	  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv);
	
 	  // Get the root POA
 	  CORBA::Object_var pfobj = orb->resolve_initial_references("RootPOA");

 	  PortableServer::POA_var rootPOA =
         PortableServer::POA::_narrow(pfobj);

 	  // Activate the root POA's manager
 	  PortableServer::POAManager_var mgr = rootPOA->the_POAManager();

 	  mgr->activate();

 	  // Create a Servant and explicitly create a CORBA object
 	  swordorb::SWMgr_impl servant(&swordmgr);
 	  CORBA::Object_var object = servant._this();

 	  // Here we get the IOR for the Hello server object.
 	  // Our "client" will use the IOR to find the object to connect to
 	  CORBA::String_var ref = orb->object_to_string( object );

 	  // print out the IOR
 	  std::cout << ref << std::endl;

 	  // run the server event loop
 	  orb->run();
  }
  catch(const CORBA::Exception& ex)
  {
    std::cout << "Exception caught." << std::endl;
  }

}
