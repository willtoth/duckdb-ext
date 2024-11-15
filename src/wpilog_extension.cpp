#define DUCKDB_EXTENSION_MAIN

#include "wpilog_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void WpilogScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Wpilog "+name.GetString()+" 🐥");;
        });
}

inline void WpilogOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Wpilog " + name.GetString() +
                                                     ", my linked OpenSSL version is " +
                                                     OPENSSL_VERSION_TEXT );;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto wpilog_scalar_function = ScalarFunction("wpilog", {LogicalType::VARCHAR}, LogicalType::VARCHAR, WpilogScalarFun);
    ExtensionUtil::RegisterFunction(instance, wpilog_scalar_function);

    // Register another scalar function
    auto wpilog_openssl_version_scalar_function = ScalarFunction("wpilog_openssl_version", {LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, WpilogOpenSSLVersionScalarFun);
    ExtensionUtil::RegisterFunction(instance, wpilog_openssl_version_scalar_function);
}

void WpilogExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string WpilogExtension::Name() {
	return "wpilog";
}

std::string WpilogExtension::Version() const {
#ifdef EXT_VERSION_WPILOG
	return EXT_VERSION_WPILOG;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void wpilog_init(duckdb::DatabaseInstance &db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::WpilogExtension>();
}

DUCKDB_EXTENSION_API const char *wpilog_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
