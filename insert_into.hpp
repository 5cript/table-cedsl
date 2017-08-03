#include "table_base.hpp"

#include <sstream>
#include <string>

namespace TableCesdl
{
    template <typename TableT, typename StreamT>
    void insertIntoQuery(StreamT& stream)
    {
        using range = boost::mpl::range_c <int, 0, boost::fusion::result_of::size <TableT>::type::value>;

        stream << "INSERT INTO " << TableT::__name << "(";
        boost::mpl::for_each <range> ([&](auto index) {
            using attribute = typename std::tuple_element <decltype(index)::value, typename TableT::attributes>::type;
            std::string name = boost::fusion::extension::struct_member_name<TableT, decltype(index)::value>::call();

            stream << name;
            if (decltype(index)::value < boost::fusion::result_of::size <TableT>::type::value - 1)
                stream << ",";
        });

        stream << ") VALUES (";
        boost::mpl::for_each <range> ([&](auto index) {
            stream << ":_" << decltype(index)::value;
            if (decltype(index)::value < boost::fusion::result_of::size <TableT>::type::value - 1)
                stream << ",";
        });
        stream << ");";
    }

    template <typename TableT>
    std::string insertIntoQuery()
    {
        std::stringstream sstr;
        insertIntoQuery <TableT>(sstr);
        return sstr.str();
    }
}
