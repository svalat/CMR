/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ENTIT_H
#define CMR_PROJECT_ENTIT_H

/********************  HEADERS  *********************/
#include <string>
#include <map>
#include <vector>
#include "../parsor/LatexEntity.h"

/********************  ENUM  ************************/
enum CMRCaptureType
{
	CMR_CAPTURE_NONE,
	CMR_CAPTURE_REQUIRED,
	CMR_CAPTURE_OPTIONS
};

/*********************  STRUCT  *********************/
struct CMRCaptureDef
{
	CMRCaptureDef(const std::string & name,CMRCaptureType captureType);
	std::string name;
	CMRCaptureType captureType;
};

/*********************  TYPES  **********************/
class CMRProjectContext;
typedef std::vector<int> CMRConstantDimensionsVector;
typedef std::vector<std::string> CMRConstantValueVector;
typedef std::vector<std::string> CMRStringVector;
typedef std::map<std::string,const CMRCompiler::LatexFormulas *> CMRProjectCaptureMap;
typedef std::vector<CMRCaptureDef> CMRProjectCaptureDefMap;

/*********************  CLASS  **********************/
class CMRProjectEntity
{
	public:
		CMRProjectEntity(const std::string & latexName,const std::string & longName);
		virtual ~CMRProjectEntity(void);
		void addIndice(const std::string & name,CMRCaptureType captureType = CMR_CAPTURE_NONE);
		void addExponent(const std::string & name,CMRCaptureType captureType = CMR_CAPTURE_NONE);
		void addParameter(const std::string & name,CMRCaptureType captureType = CMR_CAPTURE_NONE);
		void changeCaptureType(const std::string & name, enum CMRCaptureType captureType);
		bool match(const CMRCompiler::LatexEntity & entity) const;
		bool match(const std::string & value) const;
		void capture( const CMRCompiler::LatexEntity& entity, CMRProjectCaptureMap& capture ) const;
		std::string getLatexName(void) const;
		const std::string & getShortName(void) const;
		const std::string & getLongName(void) const;
		bool haveCapture( const std::string& name );
		CMRStringVector getCapturedIndices(void) const;
		const CMRProjectCaptureDefMap & getIndices(void) const;
		const CMRProjectCaptureDefMap & getExponents(void) const;
		const CMRProjectCaptureDefMap & getParameters(void) const;
		void markAllCaptureAs(enum CMRCaptureType capture);
		void captureName( void );
		bool isWildcardName(void);
		
		//to overload
		virtual void genDefinitionCCode(std::ostream& out, const CMRProjectContext& context, int padding = 0) const = 0;
		virtual void genUsageCCode(std::ostream& out, const CMRProjectContext& context, const CMRCompiler::LatexEntity& entity, bool write = false) const = 0;
		virtual void printDebug(std::ostream & out) const;
	public:
		friend std::ostream & operator << (std::ostream & out,const CMRProjectEntity & value);
	protected:
		virtual void onUpdateCaptureType(const std::string & name,enum CMRCaptureType captureType);
		bool internalMatch(const CMRCompiler::LatexEntity & entity,CMRProjectCaptureMap * capture) const;
		bool internalMatch( const CMRCompiler::LatexFormulasVector& formulaList, const CMRProjectCaptureDefMap& captureDef, CMRProjectCaptureMap* captureOut ) const;
		void applyLatexName(const std::string & latexName);
		void fillCapture(CMRProjectCaptureDefMap & capture,CMRCompiler::LatexFormulasVector & formulaList);
		void addCapture(CMRProjectCaptureDefMap & capture,const std::string & value,CMRCaptureType captureType);
		void addCapture( CMRProjectCaptureDefMap& capture, const CMRCompiler::LatexFormulas& formula, CMRCaptureType captureType );
		bool changeCaptureType(CMRProjectCaptureDefMap & capture, const std::string & name, enum CMRCaptureType captureType);
		static std::string formatCaptureList ( const CMRProjectCaptureDefMap& value, const std::string& sep, const std::string& open, const std::string& close, bool forceOpenClose);
		CMRCaptureDef * findCaptureDef( CMRProjectCaptureDefMap& value, const std::string& name, bool beCaptured = false );
		CMRCaptureDef * findCaptureDef( const std::string& name, bool beCaptured = false );
		void ensureUniqCapture( const CMRCompiler::LatexFormulas& f );
		static void printDebug( std::ostream& out, const std::string& name, const CMRProjectCaptureDefMap& map );
		static void markAllCaptureAs(CMRProjectCaptureDefMap & map,enum CMRCaptureType capture);
	private:
		std::string shortName;
		std::string longName;
		CMRProjectCaptureDefMap indices;
		CMRProjectCaptureDefMap exponents;
		CMRProjectCaptureDefMap parameters;
		bool capName;
};

#endif //CMR_PROJECT_ENTIT_H