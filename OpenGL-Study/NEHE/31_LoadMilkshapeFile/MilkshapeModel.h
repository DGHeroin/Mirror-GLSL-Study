#ifndef MILKSHAPEMODEL_H
#define MILKSHAPEMODEL_H

#include "Model.h"

class MilkshapeModel : public Model
{
	public:
		/*	���캯��. */
		MilkshapeModel();

		/*	�������� */
		virtual ~MilkshapeModel();

		/*	
			����Milkshapeģ��
		*/
		virtual bool loadModelData( const char *filename );
};

#endif // ndef MILKSHAPEMODEL_H
