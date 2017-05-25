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

Preprocess results
----
```C++
namespace Carbonide { namespace Server { namespace Database { namespace Tables {
	struct User 
	{
		boost::optional < int > UserID;
		boost::optional < std::string > Name;
		boost::optional < std::string > PassHash;
		boost::optional < std::string > Salt;
		
		using attributes = std::tuple < FieldAttributes < int, DetermineFieldType < int > ::type, SizeTypeWrapper < 0 > ::value, true, true, true > , 
										FieldAttributes < std::string, VARCHAR, SizeTypeWrapper < 255 > ::value, true, false, false > , 
										FieldAttributes < std::string, VARCHAR, SizeTypeWrapper < 255 > ::value, true, false, false > , 
										FieldAttributes < std::string, VARCHAR, SizeTypeWrapper < 255 > ::value, true, false, false > >;
										
		constexpr static const char * __name = "User";
		explicit User(boost::optional < std::string > Name = boost::none, 
					  boost::optional < std::string > PassHash = boost::none, 
					  boost::optional < std::string > Salt = boost::none)
						: Name {Name}
						, PassHash {PassHash}
						, Salt {Salt} 
		{}
		
		
		typedef struct __TableAttributes {
			constexpr static const char * charset = "utf8";
			constexpr static const char * collate = "utf8_bin"; 
			constexpr static const bool charsetSpecified = charset[0] != '\0';
			constexpr static const bool collateSpecified = collate[0] != '\0';
		} tableAttributes;
	};
}
}
}
}
```
