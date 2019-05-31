/********************************************************************
 	created:  2/8/2007  
	filename: mytest.C
  	contact:  hongpinglin@hotmail.com
	
	purpose:	
*********************************************************************/

//
// SYSTEM INCLUDE
//
 
//
// PROJECT INCLUDE
//

//
// LOCAL INCLUDE
//


#include "mysql_client_simple_conn.h"
 
using namespace sbs_mysql_simple;

 int Test1()
{
	string query="select * from ordering_error_code";

	CMysqlConn conn("sdb1.alpha.sbs.corp.yahoo.com",
					"monitoring",
					"richlin",
					"richlinpw"
					);

 
	if (conn)
	{
  		CMysqlSet rset=conn.Query<WithData>(query);

		if (rset)
		{
			cout<<"I got a result"<<endl;
 			unsigned size=rset.NumberOfRecords();
			cout<<size<<" response with "<<rset.NumberOfFields()<<" columns from the query '"<<query<<"'"<<endl;

			for (unsigned int i=0;i<size; i++)
			{
 				CMysqlRow row=rset.GetNextRow();

				if (row)
				{
					for (unsigned int j=0; j<row.NumberOfFields();j++)
					{
						cout<<"\t"<<row[j];
					}
					cout<<endl;
				}
 
 
			}
		}

		cout<<"Last error is "<<conn.GetLastErrorCode()<<":"<<conn.GetLastErrorString()<<endl;
 
 		string updateQuery="insert into ordering_error_code values (5000, 'testing message')";
		conn.Query<NoData>(updateQuery);


		//
		// Query single data test
		//
		cout<<"query single data..."<<endl;

		pair<bool, string> value=conn.Query<CheckOneRecord>("select errorcodes from ordering_error_code where errorcode='2005'");

		if (value.first)
		{
			cout<<"get value of "<<value.second<<endl;
		}

	}
	

 	return 0;
}

 
 
 

int main(int argc, char** argv)
{
 

   	Test1();
 
 
}