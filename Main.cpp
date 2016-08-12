#include <cstdlib>
#include <iostream>
#include <iomanip> // needed for setprecision
#include <string> // need to include string to have access t overloaded operators
#include <sstream>
#include <fstream>

// To start with this program will print the values of the a few Bessel functions at a given position
// The idea being to get python to call the executable
// sample call: Bessel x1 -> output: print x1 J_{0}(x1), J_{1}(x1), J_{2}(x1), J_{3}(x1)
// This program will output the values of a Bessel-J function over a given range
// sample call: Bessel order n_points x1 x2

template <class T> std::string toString(const T & t)
{
    // This is just too convenient not to use
    // Is there a version that can include something similar to %0.5d ? 
	// There has to be, look into the setw method for strings
	// Requires the string-stream (sstream) standard library
    // R. Sheehan 16 - 5 - 2011
    
    std::ostringstream oss; // create a stream
    oss << t;				// insert value to stream
    return oss.str();		// return as a string
}

template <class T> std::string toString(const T &t,int places)
{
	// toString function that allows for the
	// number of decimal places to be specified 
	// far too convenient
	// R. Sheehan 17 - 5 - 2011

	std::ostringstream oss; // create a stream

	oss<<std::fixed<<std::setprecision(places)<<t; // insert value to stream

	return oss.str(); // return as a string
}

template <class T> T DSQR(T a)
{
	// Efficient squaring operator
	// Write injuries in dust, benefits in marble
	T darg;
	return ( (darg=(a)) == (T)(0) ? (T)(0) : darg*darg );
}

static const std::string null_string="";

//cmd-line boiler plate
void cmd_input_list(int argc, char *argv[], bool loud); 

//Bessel Function of the 1st kind
double bessj0(double x);
double bessj1(double x);
double bessj(int n,double x);
double bessel_J(int n,double x); 

int main(int argc, char *argv[])
{
	try{
		
		if(argc == 2){		

			cmd_input_list(argc, argv, false); 

			// print the values of the a few Bessel functions at a given position

			double x_pos = atof( argv[1] ); // read in x-position

			if(x_pos >= 0.0){
				std::cout<<"x1 \t J_{0}(x1) \t J_{1}(x1) \t J_{2}(x1) \t J_{3}(x1)\n"; 
				std::cout<<x_pos<<" \t "<<bessel_J(0, x_pos)<<" \t "<<bessel_J(1, x_pos)<<" \t "<<bessel_J(2, x_pos)<<" \t "<<bessel_J(3, x_pos)<<"\n"; 
			}
			else{
				std::string reason = "Error: Bessel called with incorrect / invalid inputs\n";
				reason += "Input position is x = " + toString(x_pos, 3) + "\n"; 
				reason += "Input position must be greater than zero\n"; 
				throw std::invalid_argument(reason); 
			}			
		}
		else if(argc >= 5){		

			cmd_input_list(argc, argv, false); 

			// output the values of a Bessel-J function over a given range

			int order = atoi( argv[1] ); // order of Bessel function to be evaluated
			int n_points = atoi( argv[2] ); // number of points at which function is to be evaluated
			double x1 = atof( argv[3] ); 
			double x2 = atof( argv[4] ); 
			
			// test inputs to make sure they're valid
			bool c1 = order > -1 ? true : false;  
			bool c2 = n_points > -1 ? true : false; 
			bool c3 = x1 >= 0.0 ? true : false; 
			bool c4 = x2 >= 0.0 ? true : false; 

			if(c1 && c2 && c3 && c4){			

				// compute spacing
				double start_pos = std::min(x1, x2);
				double dx = (std::max(x1, x2) - start_pos ) / (static_cast<double>(n_points-1)); 
				
				std::string filename = "Bessel_" + toString(order) + "_Values.txt";
				std::ofstream write(filename, std::ios_base::out|std::ios_base::trunc); 

				if( write.is_open() ){

					for(int i=1; i<=n_points; i++){
						write<<std::setprecision(10)<<start_pos<<" , "<<bessel_J(order, start_pos)<<"\n"; 
						start_pos += dx; 
					}
					
					write.close(); 
				}
			}
			else{
				// one or more of the inputs was invalid
				std::string reason = "Error: Bessel called with incorrect / invalid inputs\n";
				if(!c1) reason += "Input order n = " + toString(order) + " is negative\n"; 
				if(!c1) reason += "Input num-points = " + toString(n_points) + " is negative\n"; 
				if(!c1) reason += "Input x1 = " + toString(x1,3) + " is negative\n"; 
				if(!c1) reason += "Input x2 = " + toString(x2,3) + " is negative\n"; 
				throw std::invalid_argument(reason); 
			}
		}
		else{
			std::string reason = "Error: Bessel called with incorrect / invalid inputs\n"; 
		
			reason += toString(argc-1) + " parameters were input into the program\n"; 
		
			for(int count = 1; count < argc; count++){
				reason += "argv[" + toString(count) + "] = " + argv[count] + "\n"; 
			}

			throw std::invalid_argument(reason); 
		}
	}
	catch(std::invalid_argument &e){
		
		std::cerr<<e.what()<<"\n"; 
		
		exit(EXIT_FAILURE); 
	}

	return 0; 
}

void cmd_input_list(int argc, char *argv[], bool loud)
{
	// print the listed cmd line inputs to the console
	// List off the input parameters
	// Program needs 4 or more parameters to run, remember that the name of the program is also considered a parameter
	// argv[0] = program name

	if(loud){
		std::cout<<argc-1<<" parameters were input into the program\n"; 
		for(int count = 1; count < argc; count++){
			std::cout<<"argv["<<count<<"] = "<<argv[count]<<"\n"; 
		}
		std::cout<<"\n";
	}
}

double bessj0(double x)
{
	//Return the Bessel Function J0(x) for all real x
	double z,ax,xx,y,ans,ans1,ans2;

	if((ax=fabs(x))<8.0){
		y=DSQR(x);
		ans1=57568490574.0+y*(-13362590354.0+y*(651619640.7+
			y*(-11214424.18+y*(77392.33017+y*(-184.9052456)))));
		ans2=57568490411.0+y*(1029533985.0+y*(9494680.718+
			y*(59272.64853+y*(267.8532712+y))));
		ans=ans1/ans2;
	}
	else{
		z=8.0/ax;
		y=DSQR(z);
		xx=ax-0.785398164;
		ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4+
			y*(-0.2073370639e-5+y*0.2093887211e-6)));
		ans2=-0.1562499995e-1+y*(0.1430488765e-3+
			y*(-0.6911147651e-5+y*(0.7621095161e-6-y*0.934945152e-7)));
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
	}
	return ans;
}

double bessj1(double x)
{
	//Returns the Bessel function J1(x) for all real x
	double ax,z,xx,y,ans,ans1,ans2;

	if((ax=fabs(x))<8.0){
		y=DSQR(x);
		ans1=x*(72362614232.0+y*(-7895059235.0+y*(242396853.1
			+y*(-2972611.439+y*(15704.48260+y*(-30.16036606))))));
		ans2=144725228442.0+y*(2300535178.0+y*(18583304.74
			+y*(99447.43394+y*(376.9991397+y))));
		ans=ans1/ans2;
	}
	else{
		z=8.0/ax;
		y=DSQR(z);
		xx=ax-2.356194491;
		ans1=1.0+y*(0.183105e-2+y*(-0.3516396496e-4
			+y*(0.245752017e-5+y*(-0.240337019e-6))));
		ans2=0.04687499995+y*(-0.2002690873e-3
			+y*(0.8449199096e-5+y*(-0.88228987e-6
			+y*0.105787412e-6)));
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
		if(x<0.0) ans=-ans;
	}
	return ans;
}

double bessj(int n,double x)
{
	//Returns te Bessel function Jn(x) for all real x and n >= 2
	int j,jsum,m;
	double ax,bj,bjm,bjp,sum,tox,ans;

	static const double ACC=40.0;
	static const double BIGNO=1e10;
	static const double BIGNI=1.0e-10;

	if(n<2){
		std::cerr<<"Index n less than 2 in bessj\n";	
	}
	ax=fabs(x);
	if(ax==0.0){
		return 0.0;	
	}
	else if(ax>static_cast<double>(n)){
		//Upwards recurrence from J0 and J1
		tox=2.0/ax;
		bjm=bessj0(ax);
		bj=bessj1(ax);
		for(j=1;j<n;j++){
			bjp=j*tox*bj-bjm;
			bjm=bj;
			bj=bjp;
		}
		ans=bj;
	}
	else{
		//Downwards recurrence from an even m here computed
		tox=2.0/ax;
		m=2*((n+static_cast<int>(sqrt(ACC*n)))/2);
		jsum=0;
		bjp=ans=sum=0.0;
		bj=1.0;
		for(j=m;j>0;j--){
			bjm=j*tox*bj-bjp;
			bjp=bj;
			bj=bjm;
			if(fabs(bj)>BIGNO){
				bj*=BIGNI;
				bjp*=BIGNI;
				ans*=BIGNI;
				sum*=BIGNI;
			}
			if(jsum) sum+=bj;
			jsum=!jsum;
			if(j==n) ans=bjp;
		}
		sum=2.0*sum-bj;
		ans/=sum;
	}
	return x<0.0 && (n&1) ? -ans:ans; 
}

double bessel_J(int n,double x)
{
	if(n==0){
		return bessj0(x);	
	}
	else if(n==1){
		return bessj1(x);	
	}
	else{
		return bessj(n,x);	
	}
}