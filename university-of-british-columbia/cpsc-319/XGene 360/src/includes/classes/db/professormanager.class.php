<?php

require_once( 'usermanager.class.php' );

class ProfessorManager extends UserManager
{
	var $m_obj_user = null;
	var $m_obj_db = null;

	/**
	 * Class Constructor
	 * PRE: $user_class is a valid user class enumerated in the id in the User table
	 * POST: TA object is constructed
	 * @param string $p_userId
	 */
	function ProfessorManager( $obj_user, $obj_db )
	{
		$this->m_obj_user = $obj_user;
		$this->m_obj_db = $obj_db;
	}

	function view_professors()
	{
		$str_sql_query = null;
	  
		switch ( $this->m_obj_user->int_privilege )
		{
			case UP_PROFESSOR:
			// prof sees all prof for now
			case UP_ADMINISTRATOR:
			   
			    $str_sql_query = "SELECT U.UserId, U.FirstName, U.LastName "
							   . "FROM User U "
							   . "WHERE U.PrivilegeLvl = " . UP_PROFESSOR . " "
							   . "ORDER BY U.LastName, U.FirstName, U.UserId";
				break;
		}
	
		if ( $str_sql_query == null )
		{
			// TODO: fix up Error Handler or Message Handler
			
			return null;
		}
	  
		return $this->m_obj_db->query_select( $str_sql_query );
	}
	
	function view_professor_courses( $str_professor_id )
	{
		$str_sql_query = null;
	  
		switch ( $this->m_obj_user->int_privilege )
		{
			case UP_PROFESSOR:
				
			    $str_sql_query = "SELECT C.CourseId, C.Name, C.Description "
							   . "FROM User U, Course C, XG_Teaches T, User Current, XG_Teaches T2 "
							   . "WHERE C.CourseId = T.course_id "
							   . "AND T.professor_id = U.UserId "
							   . "AND T2.professor_id = Current.UserId "
							   . "AND T.course_id = T2.course_id "
							   . "AND U.UserId = '". $this->m_obj_db->format_sql_string( $str_professor_id ) ."' "
							   . "AND Current.UserId = '" . $this->m_obj_db->format_sql_string( $this->m_obj_user->str_username ) . "' "
							   . "ORDER BY C.Name, C.Description";
				break;
			
			
			case UP_ADMINISTRATOR:
			   
			    $str_sql_query = "SELECT C.CourseId, C.Name, C.Description "
							   . "FROM User U, Course C, XG_Teaches T "
							   . "WHERE C.CourseId = T.course_id "
							   . "AND T.professor_id = U.UserId "
							   . "AND U.UserId = '". $this->m_obj_db->format_sql_string( $str_professor_id ) ."' "
							   . "ORDER BY C.Name, C.Description";
				break;
		}
	
		if ( $str_sql_query == null )
		{
			// TODO: fix up Error Handler or Message Handler
			
			return null;
		}
	  
		return $this->m_obj_db->query_select( $str_sql_query );
	}
}

?>