/********************************************************************
 	created:  2/7/2007  
	filename: mysql_client_simple_conn.h
  	contact:  hongpinglin@hotmail.com
	
	purpose:	
*********************************************************************/
#ifndef _mysql_client_simple_conn_h_2_7_2007
#define _mysql_client_simple_conn_h_2_7_2007

#include <string>
#include <sstream>
#include <stdexcept> 

#include <boost/shared_ptr.hpp>
#include <mysql/mysql.h>

#include "mysql_client_simple_private.h"

namespace sbs_mysql_simple
{

	typedef boost::shared_ptr<MYSQL> ConnPtr;
	typedef boost::shared_ptr<MYSQL_RES> ResPtr;

	class CMysqlSet;

//
// The thin wrapper of MYSQL_ROW
//
	class CMysqlRow
	{
	public:

		friend class CMysqlSet;


		inline operator bool () const 
		{
			return m_ptr&&m_row;
		}


		string operator [] (unsigned int index)
		{
 			if (index >= m_nFields)
				throw std::logic_error ("The invalid field index ");
			return string(m_row[index]);
		}
 
 
		unsigned int NumberOfFields() const {return  m_nFields;}

	private:
		CMysqlRow():m_row(NULL),m_nFields(0){}

		CMysqlRow (ResPtr ptr, unsigned int nfields)
			:m_ptr(ptr),m_row(NULL),m_nFields(nfields)
		{
			if (m_ptr)
			{
				m_row=mysql_fetch_row(m_ptr.get());
			}
		}

 
	private:

		ResPtr m_ptr;
 		MYSQL_ROW m_row;
		size_t m_nFields;
	};

//
// The thin wrapper of MYSQL_RES
//

	class CMysqlSet
	{
	public:

		CMysqlSet():m_rowCount(0),m_nFields(0){}


		CMysqlSet(MYSQL_RES* res)
			:m_ptr(res,myprivate::CloseResultSet),m_rowCount(0),m_nFields(0)
		{
			if (m_ptr)
			{
				m_rowCount=mysql_num_rows(m_ptr.get());
				m_nFields=mysql_num_fields(m_ptr.get());
			}
		}

 
		
		inline operator bool () const {return m_ptr;}

		size_t NumberOfRecords() const {return m_rowCount;}

		unsigned int NumberOfFields() const {return  m_nFields;}

		CMysqlRow GetNextRow()
		{
			  return CMysqlRow(m_ptr,NumberOfFields());
		}

	private:
		
		ResPtr m_ptr;
		size_t m_rowCount;
		size_t m_nFields;
		
	};


	//
	// Declare and define the ProcessQueryPolicy
	//
	//

	//
	// The policy for the not data query such as "update", "insert" statement
	//
	//
	struct NoData
	{
		typedef bool ReturnType;
		static ReturnType ReturnInFail()  {return false;}
		static ReturnType DeepQuery(ConnPtr ptr) {return true;}
	};


	//
	// The policy for the query with entire dataset
	//
	struct WithData
	{
		typedef CMysqlSet ReturnType;

		static ReturnType ReturnInFail()  {return CMysqlSet();}
		
		static ReturnType DeepQuery(ConnPtr ptr) 
		{
			MYSQL_RES* res_set=mysql_store_result(ptr.get());
			if (!res_set) return CMysqlSet();
			return CMysqlSet(res_set);
		}
	};


	//
	// The policy to check one field of the first record
	// which is normally used in checking if the valid record exist
	//
	struct CheckOneRecord
	{
		typedef pair<bool, string> ReturnType;

		static ReturnType ReturnInFail()  {return ReturnType(false,"no record");}
		
		static ReturnType DeepQuery(ConnPtr ptr) 
		{
			MYSQL_RES* res_set=mysql_store_result(ptr.get());
			if (!res_set) return ReturnType(false,"no record");
			
			CMysqlSet rset(res_set);

			size_t size=rset.NumberOfRecords();

			if (size)
			{
				CMysqlRow row=rset.GetNextRow();

				if (row.NumberOfFields()>0)
				{
					string value=row[0];
					return ReturnType(true,value);
				}
			}

			return ReturnType(false,"no record");
		}
	};





	
 //
// The thin wrapper of MYSQL connection
//
	class CMysqlConn 
	{
	public:

		CMysqlConn(){}

		CMysqlConn (string const& hostName,  
					string const& dbName, 
					string const&username, 
					string const& password)
		{
			Connect(hostName,dbName,username,password);
		}

		bool Connect (string const& hostName,  
						string const& dbName,  
						string const&username, 
						string const& password)
		{
			bool ret=true;
			m_ptr.reset();
 			try
			{
				m_ptr=myprivate::
						GetConnection(hostName,dbName,username,password);
			}
			catch (runtime_error & e)
			{
				cerr<<e.what()<<endl;
				ret=false;
			}
			return ret;
		}


 		inline operator bool() const {return m_ptr;}


 		template <typename ProcessQuery>
			typename ProcessQuery::ReturnType Query (string const& sql)
		{
 			typedef typename ProcessQuery::ReturnType ReturnType;

			if (!m_ptr)
			{
 				return ProcessQuery::ReturnInFail();
			}
			
 			cerr<<"Query '"<<sql<<"' ";
 
			if (mysql_query(m_ptr.get(),sql.c_str())!=0)
			{
 					cerr<<"Fail "
					<<GetLastErrorCode()
					<<":"
					<<GetLastErrorString()
					<<" - for the query of '"
 					<<sql
					<<"'"
					<<endl;
  				return ProcessQuery::ReturnInFail();
			}
			else
			{
				cerr<<"Success."<<endl;
			}
 
 			return ProcessQuery::DeepQuery(m_ptr);
		}

 


		int GetLastErrorCode() const 
		{return myprivate::GetLastErrorCode(m_ptr);}
		
		string GetLastErrorString() const
		{return myprivate::GetLastErrorString(m_ptr);}

 		
	private:
 		ConnPtr m_ptr;
	};
};


#endif
