#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class LOTable
{
   public:
      struct Item
      {
         Item( float tmpmass, float tmpr2up, float tmpr2uperror, float tmpr2, float tmpr2error, float tmpr2down, float tmpr2downerror) : mass(tmpmass), r2up(tmpr2up), r2uperror(tmpr2uperror), r2(tmpr2), r2error(tmpr2error), r2down(tmpr2down), r2downerror(tmpr2downerror) {}
         float mass, r2up, r2uperror, r2, r2error, r2down, r2downerror;
      };

      LOTable() {}

      void LoadTable(string filename);

      vector<float> GetValue( float mass);

   private:
      vector< Item > thetable;
};

void LOTable::LoadTable(string filename)
{
   string line;
   ifstream myfile (filename.c_str());
   float tmpmass, tmpr2up, tmpr2uperror, tmpr2, tmpr2error, tmpr2down, tmpr2downerror;

   if (myfile.is_open())
   {
      cout << filename << endl;

      while ( myfile.good() )
      {
         myfile >> tmpmass >> tmpr2up >> tmpr2uperror >> tmpr2 >> tmpr2error >> tmpr2down >> tmpr2downerror;

         Item arow(tmpmass, tmpr2up, tmpr2uperror, tmpr2, tmpr2error, tmpr2down, tmpr2downerror);
         thetable.push_back( arow);

      }
   }
   myfile.close();

}

vector<float> LOTable::GetValue( float mass)
{
   vector<float> result;
   float tmpresult[3] = {1.0,1.0,1.0};

   for ( vector<Item>::iterator it = thetable.begin(); it < thetable.end(); it++ )
   {
      float tmpmass1 = it->mass;
      vector<Item>::iterator tmpit = (it + 1);
      float tmpmass2 = tmpit->mass;

      tmpresult[1] = it->r2;
      tmpresult[0] = it->r2up;
      tmpresult[2] = it->r2down;

      if ( ( tmpmass1 <= mass && mass < tmpmass2 ) )
      {  
         for(int i= 0; i < 3; i++)
         {
            result.push_back(tmpresult[i]);
         }

         return result; 
      }
   }


   vector<Item>::iterator itbegin = thetable.begin();
   vector<Item>::iterator itend = (thetable.end() - 1);

   //Smaller than First mass in the Table
   if (mass < itbegin->mass)
   {
      tmpresult[1] = itbegin->r2;
      tmpresult[0] = itbegin->r2up;
      tmpresult[2] = itbegin->r2down;

      for(int i= 0; i < 3; i++)
      {

         result.push_back(tmpresult[i]);
      }

      return result;
   }

   //Larger than last mass in the Table
   if ( mass >= itend->mass ) 
   {
      tmpresult[1] = itend->r2;
      tmpresult[0] = itend->r2up;
      tmpresult[2] = itend->r2down;

      for(int i= 0; i < 3; i++)
      {
         result.push_back(tmpresult[i]);

      }

      return result; 
   };

}
