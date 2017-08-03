#pragma once

#include "table_base.hpp"

#include <vector>
#include <string>
#include <sstream>

namespace TableCesdl
{
    template <typename StreamT, typename ForeignKeyDesc>
    struct foreignKeyEx
    {
        static bool op(StreamT& stream, std::string const& name)
        {
            stream << "FOREIGN KEY(" << name << ") REFERENCES " << ForeignKeyDesc::table::c_str << "(" << ForeignKeyDesc::key::c_str << ")";
            return true;
        }
    };

    template <typename StreamT>
    struct foreignKeyEx <StreamT, TableCesdl::NotForeignKey>
    {
        static bool op(StreamT& stream, ...)
        {
            return false;
        }
    };

    template <typename TableT, typename StreamT>
    void createTableQuery(StreamT& stream, bool ignoreExisting = true, bool ignoreTableAttributes = false)
    {
        using range = boost::mpl::range_c <int, 0, boost::fusion::result_of::size <TableT>::type::value>;

        stream << "CREATE TABLE ";
        if (ignoreExisting)
            stream << "IF NOT EXISTS ";

        stream << TableT::__name;

        stream << " (\n";

        boost::mpl::for_each <range> ([&](auto index) {
            using attribute = typename std::tuple_element <decltype(index)::value, typename TableT::attributes>::type;
            std::string name = boost::fusion::extension::struct_member_name<TableT, decltype(index)::value>::call();

            stream << "\t" << name << " " << attribute::type::getName();
            if (attribute::size != 0) {
                stream << "(" << attribute::size << ")";
            }

            if (attribute::notNull)
                stream << " NOT NULL";

            if (attribute::autoIncrement)
                stream << " AUTO_INCREMENT";

            if (attribute::primaryKey)
                stream << " PRIMARY KEY";

            foreignKeyEx <StreamT, typename attribute::references>::op(stream, name);

            if (decltype(index)::value < boost::fusion::result_of::size <TableT>::type::value - 1)
                stream << ",";

            stream << "\n";
        });

        stream << ")";

        if (TableT::tableAttributes::charsetSpecified && !ignoreTableAttributes)
            stream << " CHARACTER SET " << TableT::tableAttributes::charset;

        if (TableT::tableAttributes::collateSpecified && !ignoreTableAttributes)
            stream << " COLLATE " << TableT::tableAttributes::collate;
    }

    template <typename T>
    std::string createTableQuery(bool ignoreExisting = true, bool ignoreTableAttributes = false)
    {
        std::stringstream sstr;
        createTableQuery <T>(sstr, ignoreExisting, ignoreTableAttributes);
        return sstr.str();
    }

} // namespace TableCesdl
