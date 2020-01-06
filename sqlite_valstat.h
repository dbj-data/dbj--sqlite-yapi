/* (c) 2019 by dbj.org   -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ */

#ifndef _DBJ_SQLITE_VALSTAT_INC_
#define _DBJ_SQLITE_VALSTAT_INC_

#include "../dbj--nanolib/dbj++valstat.h"

namespace dbj::sql
{
	// using namespace std;
	using buffer = typename dbj::nanolib::v_buffer;
	using status_type  = dbj::nanolib::status_type ;

	constexpr const char* category_name() { return "sqlite3"; }

		enum class sqlite_status_code : int
		{
			/*
				https://sqlite.org/c3ref/c_abort.html
				note:
				this must match with sqlite3 # defines
			*/
			sqlite_ok = SQLITE_OK,	/* successful result */
			sqlite_error = SQLITE_ERROR, /* generic error */
			sqlite_internal = SQLITE_INTERNAL, /* internal logic error in sqlite */
			sqlite_perm = SQLITE_PERM, /* access permission denied */
			sqlite_abort = SQLITE_ABORT, /* callback routine requested an abort */
			sqlite_busy = SQLITE_BUSY, /* the database file is locked */
			sqlite_locked = SQLITE_LOCKED, /* a table in the database is locked */
			sqlite_nomem = SQLITE_NOMEM, /* a malloc() failed */
			sqlite_readonly = SQLITE_READONLY, /* attempt to write a readonly database */
			sqlite_interrupt = SQLITE_INTERRUPT, /* operation terminated by sqlite= 3 _interrupt()*/
			sqlite_ioerr = SQLITE_IOERR, /* some kind of disk i/o error occurred */
			sqlite_corrupt = SQLITE_CORRUPT, /* the database disk image is malformed */
			sqlite_notfound = SQLITE_NOTFOUND, /* unknown opcode in sqlite= 3 _file_control() */
			sqlite_full = SQLITE_FULL, /* insertion failed because database is full */
			sqlite_cantopen = SQLITE_CANTOPEN, /* unable to open the database file */
			sqlite_protocol = SQLITE_PROTOCOL, /* database lock protocol error */
			sqlite_empty = SQLITE_EMPTY, /* internal use only */
			sqlite_schema = SQLITE_SCHEMA, /* the database schema changed */
			sqlite_toobig = SQLITE_TOOBIG, /* string or blob exceeds size limit */
			sqlite_constraint = SQLITE_CONSTRAINT, /* abort due to constraint violation */
			sqlite_mismatch = SQLITE_MISMATCH, /* data type mismatch */
			sqlite_misuse = SQLITE_MISUSE, /* library used incorrectly */
			sqlite_nolfs = SQLITE_NOLFS, /* uses os features not supported on host */
			sqlite_auth = SQLITE_AUTH, /* authorization denied */
			sqlite_format = SQLITE_FORMAT, /* not used */
			sqlite_range = SQLITE_RANGE, /* = 2 nd parameter to sqlite= 3 _bind out of range */
			sqlite_notadb = SQLITE_NOTADB, /* file opened that is not a database file */
			sqlite_notice = SQLITE_NOTICE, /* notifications from sqlite= 3 _log() */
			sqlite_warning = SQLITE_WARNING, /* warnings from sqlite= 3 _log() */
			sqlite_row = SQLITE_ROW, /* sqlite= 3 _step() has another row ready */
			sqlite_done = SQLITE_DONE        /* sqlite= 3 _step() has finished executing */
		};                          // dbj_status_code/*

			/*
			The sqlite3_errstr() interface valstat the English-language text that
			describes the result code, as UTF-8. Memory to hold the error message
			string is managed internally and must not be freed by the application.

			function argument is int so that native sqlite3 return values can be
			used also
			*/
		inline status_type err_message_sql(int sqlite_return_value)  noexcept
		{
			if (const char* mp_ = ::sqlite::sqlite3_errstr(sqlite_return_value); mp_ != nullptr)
				return buffer::make(mp_);
			else
				return buffer::make("Unknown SQLITE status code");
		}

		// this overload is required by nanolib valstat service
		inline status_type err_message_sql(sqlite_status_code code_)  noexcept
		{
			return err_message_sql(int(code_));
		}
		
		inline int code_to_int(sqlite_status_code code_)  noexcept
		{
			return int(code_);
		}

		/*
		this is the sqlite3 logic: not all codes are errors
		*/
		inline constexpr bool is_sqlite_error(sqlite_status_code sc_) noexcept
		{
			switch (sc_) {
			case sqlite_status_code::sqlite_ok:
			case sqlite_status_code::sqlite_row:
			case sqlite_status_code::sqlite_done:
				return false;
			default:
				return true;
			};
		}

		// sqlite api returns int as status codes basically
		inline constexpr bool is_sqlite_error(int sqlite3_result_) noexcept
		{
			return is_sqlite_error(sqlite_status_code (sqlite3_result_));
		}

/*
--------------------------------------------------------------------------------------------
*/
			template < typename T_ >
			using dbj_sql_valstat =  dbj::valstat< T_ , sqlite_status_code >;
/*
--------------------------------------------------------------------------------------------
*/

	inline constexpr bool is_error	(sqlite_status_code const& sqlite3_rt)
	{
		return is_sqlite_error( sqlite3_rt );
	}

} // namespace dbj::sql


#endif // !_DBJ_SQLITE_VALSTAT_INC_
