# table-cedsl
An embedded language for Data Description Language. (Header only)
Meant to be used in combination with SOCI.

## How it looks

```C++
#include <string>
#include <sstream>
#include <iostream>

#include "table-cedsl/table_base.hpp"
#include "table-cedsl/table_create.hpp"

// namespace ...
DECLARE_TABLE_3
(
    // Table class "User" in namespace Carbonide::Server::Database::Tables
    User, (Carbonide)(Server)(Database)(Tables),
    (int, UserID, NOT_NULL, PRIMARY_KEY, AUTO_INCREMENT)
    (std::string, Name, TYPE(VARCHAR), SIZE(255), NOT_NULL)
    (std::string, PassHash, TYPE(VARCHAR), SIZE(255), NOT_NULL)
    (std::string, Salt, TYPE(VARCHAR), SIZE(255), NOT_NULL)
    ,
    (
        CHARACTER_SET("utf8"),
        COLLATE("utf8_bin")
    )
)

int main()
{
	std::stringstream query;
	TableCesdl::createTableQuery </*namespace::*/ User> (query);
	std::cout << query.str() << "\n";
	return 0;
}
```
