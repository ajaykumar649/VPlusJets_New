/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooExpPoly.h" 
#include "RooMath.h"
#include "RooComplex.h"
#include "TMath.h" 

ClassImp(RooExpPoly) 
;

RooExpPoly::RooExpPoly(const char *name, const char *title, 
		       RooAbsReal& _x,
		       RooArgList& _coefs) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  coefs("coefs","coefs",this)
{
  TIterator *cx = _coefs.createIterator();
  RooAbsReal *coef;
  while ((coef = (RooAbsReal*)cx->Next())) {
    if (!dynamic_cast<RooAbsReal*>(coef)) {
      cerr << "Coefficient " << coef->GetName() << " is not good." << endl;
      assert(0);
    }
    coefs.add(*coef);
  }
  delete cx;
}


RooExpPoly::RooExpPoly(const RooExpPoly& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  coefs("coefs",this,other.coefs)
{ 
} 



Double_t RooExpPoly::evaluate() const 
{ 
  // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
  Double_t exponent(0.);
  Int_t order(0);
  RooAbsReal *coef;
  for (order = 1; order <= coefs.getSize(); ++order) {
    coef = dynamic_cast<RooAbsReal *>(coefs.at(order-1));
    exponent += coef->getVal()*TMath::Power(x, order);
  }
  return TMath::Exp(exponent) ; 
} 

Int_t RooExpPoly::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars,
					const char */*rangeName*/) const {
  if ( (coefs.getSize()<2) && (matchArgs(allVars, analVars, x)) )
    return 1;
  if ( (coefs.getSize()==2) && 
       // (dynamic_cast<RooAbsReal *>(coefs.at(1))->getVal() <= 0) && 
       (matchArgs(allVars, analVars, x)) )
    return 1;

  return 0;
}

Double_t RooExpPoly::analyticalIntegral(Int_t code, 
					const char *rangeName) const {
  assert(code);
  double coef1(0.), coef2(0.);
  static Double_t const rootpi(TMath::Sqrt(TMath::Pi()));
  
  Double_t val(x.max(rangeName) - x.min(rangeName));
  if (coefs.getSize() > 0) {
    coef1 = dynamic_cast<RooAbsReal *>(coefs.at(0))->getVal();
    if (coef1 != 0.)
      val = (TMath::Exp(coef1*x.max(rangeName)) -
	     TMath::Exp(coef1*x.min(rangeName)))/coef1;
    if (coefs.getSize() > 1) {
      coef2 = dynamic_cast<RooAbsReal *>(coefs.at(1))->getVal();
      if (coef2 != 0) {
	// std::cout << "coef1: " << coef1 << " coef2: " << coef2 << '\n';
	RooComplex c1(coef1, 0); RooComplex c2(coef2, 0);
	RooComplex rootc2((coef2 > 0) ? RooComplex(TMath::Sqrt(coef2),0) :
			  RooComplex(0,TMath::Sqrt(-1*coef2)));
	RooComplex zmax = c1*0.5/rootc2 + rootc2*x.max(rangeName);
	RooComplex zmin = c1*0.5/rootc2 + rootc2*x.min(rangeName);
	RooComplex zval_max = 
	  erfi(zmax)*0.5*rootpi*TMath::Exp(-coef1*coef1/4./coef2)/rootc2;
	RooComplex zval_min = erfi(zmin)*0.5*rootpi*TMath::Exp(-coef1*coef1/4./coef2)/rootc2;
	// std::cout << "c1: "; c1.Print();
	// std::cout << "c2: "; c2.Print();
	// std::cout << "rootc2: "; rootc2.Print();
	// std::cout << "zmax: "; zmax.Print();
	// std::cout << "zval_max: "; zval_max.Print();
	// std::cout << "zmin: "; zmin.Print();
	// std::cout << "zval_min: "; zval_min.Print();
	val = zval_max.re() - zval_min.re();
      }
    }
  }
  return val;
}

RooComplex RooExpPoly::erfi(RooComplex z) {
  static RooComplex myi(0,1);
  static RooComplex myone(1,0);

  RooComplex zsq(z*z);
  RooComplex ret(1,0);
  // std::cout << "z: "; z.Print();
  if (z.im() > 10.)
    ret =  RooComplex(0., 1.);
  else if (z.im() < -10.)
    ret =  RooComplex(0.,-1.);
  else {
    RooComplex w(RooMath::ComplexErrFunc(z));
    // std::cout << "w(z): "; w.Print();
    // std::cout << "zsq: "; zsq.Print();
    // std::cout << "zsq.exp(): "; zsq.exp().Print();
    ret = myi*(myone - ((zsq.exp())*(w)));
  }

  // std::cout << "erfi: "; ret.Print();
  return ret;
}
