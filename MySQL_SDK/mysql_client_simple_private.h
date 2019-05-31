/********************************************************************
 	created:  2/8/2007  
	filename: mysql_client_simple_private.h
  	contact:  richlin@yahoo-inc.com
	
	purpose:	
*********************************************************************/
#ifndef _mysql_client_simple_private_h_2_8_2007
#define _mysql_client_simple_private_h_2_8_2007

#include <string>
#include <sstream>
#include <stdexcept> 
#include <boost/shared_ptr.hpp>
#include <mysql/mysql.h>

	namespace myprivate
	{
		typedef boost::shared_ptr<MYSQL> ConnPtr;
		typedef boost::shared_ptr<MYSQL_RES> ResPtr;
		
		//
		// Try to use this close function instead of 'mysql_close' to 
		// avoid dependency of the mysqlclient library and increase 
		// debuggability
		//
		static	void CloseConnection(MYSQL* conn)
		{
			if (conn)
			{
					mysql_close(conn);
//					cerr<<"mysql_close is called"<<endl;
			}
					
		}


		static	void CloseResultSet(MYSQL_RES* res)
		{
			if (res)
			{
					mysql_free_result(res);
//					cerr<<"CloseResultSet is called"<<endl;
			}
					
		}

		
		//
		// This function may throw run time error
		//
		static ConnPtr GetConnection(string const& hostName,  string const& dbName, string const&username, string const& password)
		{
			MYSQL* conn=mysql_init(NULL);

			if (!conn) 
				throw std::runtime_error ("Unable to initialize the connection in mysql_init");


			boost::shared_ptr<MYSQL> shared_conn(conn,CloseConnection);

 
			if (mysql_real_connect(conn,
								hostName.c_str(),
								username.c_str(),
								password.c_str(),
								dbName.c_str(),
								0, // default port
								NULL,// default socket name,
								0 //connection flag, none
								)==NULL)
			{
				ostringstream os;
				os<<"Unable to connect db '"<<dbName<<"'"<<" of "<<hostName;
				throw std::runtime_error (os.str());
			}

			return shared_conn;
		}

		static int GetLastErrorCode(ConnPtr ptr) {return mysql_errno(ptr.get());}
		static string GetLastErrorString(ConnPtr ptr) {return mysql_error(ptr.get());}

	};
#endif
