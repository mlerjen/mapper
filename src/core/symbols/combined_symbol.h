/*
 *    Copyright 2012, 2013 Thomas Schöps
 *    Copyright 2012-2017 Kai Pastor
 *
 *    This file is part of OpenOrienteering.
 *
 *    OpenOrienteering is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    OpenOrienteering is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with OpenOrienteering.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef OPENORIENTEERING_COMBINED_SYMBOL_H
#define OPENORIENTEERING_COMBINED_SYMBOL_H

#include "symbol.h"

#include <vector>

#include <Qt>

class QIODevice;
class QXmlStreamReader;
class QXmlStreamWriter;

class Map;
class MapColor;
class MapColorMap;
class Object;
class ObjectRenderables;
class PathObject;
class PathPartVector;
class SymbolPropertiesWidget;
class SymbolSettingDialog;
class VirtualCoordVector;


/**
 * Symbol which can combine other line and area symbols,
 * creating renderables for each of them.
 * 
 * To use, set the number of parts with setNumParts() and set the indivdual part
 * pointers with setPart(). Parts can be private, i.e. the CombinedSymbol owns
 * the part symbol and it is not entered in the map as an individual symbol.
 */
class CombinedSymbol : public Symbol
{
friend class CombinedSymbolSettings;
friend class PointSymbolEditorWidget;
friend class OCAD8FileImport;
public:
	CombinedSymbol();
	virtual ~CombinedSymbol();
	Symbol* duplicate(const MapColorMap* color_map = nullptr) const override;
	
	bool validate() const override;
	
	void createRenderables(
	        const Object *object,
	        const VirtualCoordVector &coords,
	        ObjectRenderables &output,
	        Symbol::RenderableOptions options) const override;
	
	void createRenderables(
	        const PathObject* object,
	        const PathPartVector& path_parts,
	        ObjectRenderables &output,
	        Symbol::RenderableOptions options) const override;
	
	void colorDeleted(const MapColor* color) override;
	bool containsColor(const MapColor* color) const override;
	const MapColor* guessDominantColor() const override;
	bool symbolChanged(const Symbol* old_symbol, const Symbol* new_symbol) override;
	bool containsSymbol(const Symbol* symbol) const override;
	void scale(double factor) override;
	Type getContainedTypes() const override;
	
	bool loadFinished(Map* map) override;
	
    float calculateLargestLineExtent(Map* map) const override;
	
	// Getters / Setter
	inline int getNumParts() const {return (int)parts.size();}
	inline void setNumParts(int num) {parts.resize(num, nullptr); private_parts.resize(num, false);}
	
	inline const Symbol* getPart(int i) const {return parts[i];}
	void setPart(int i, const Symbol* symbol, bool is_private);
	
	inline bool isPartPrivate(int i) const {return private_parts[i];}
	inline void setPartPrivate(int i, bool set_private) {private_parts[i] = set_private;}
	
	SymbolPropertiesWidget* createPropertiesWidget(SymbolSettingDialog* dialog) override;
	
protected:
#ifndef NO_NATIVE_FILE_FORMAT
	bool loadImpl(QIODevice* file, int version, Map* map) override;
#endif
	void saveImpl(QXmlStreamWriter& xml, const Map& map) const override;
	bool loadImpl(QXmlStreamReader& xml, const Map& map, SymbolDictionary& symbol_dict) override;
	bool equalsImpl(const Symbol* other, Qt::CaseSensitivity case_sensitivity) const override;
	
	std::vector<bool> private_parts;
	std::vector<const Symbol*> parts;
	std::vector<int> temp_part_indices;	// temporary vector of the indices of the 'parts' symbols, used just for loading
};

#endif