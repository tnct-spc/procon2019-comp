#ifndef FIXEDFIELDSORTEDPOSSIMPLECONVERTER_H
#define FIXEDFIELDSORTEDPOSSIMPLECONVERTER_H

#include "FieldConverter/fieldconverterwrapper.h"

class FixedFieldSortedPosSimpleConverter : public FieldConverterWrapper
{
public:
    FixedFieldSortedPosSimpleConverter(const procon::Field& field);
};

#endif // FIXEDFIELDSORTEDPOSSIMPLECONVERTER_H
