<?php

namespace App\Model;

use Nette;

/**
 * Users management.
 */
class UserManager extends Nette\Object implements Nette\Security\IAuthenticator
{
	const
		TABLE_NAME = 'Airport_employee',
		COLUMN_LOGIN = 'login',
		COLUMN_PASSWORD = 'password',
		COLUMN_ROLE = 'role',
		COLUMN_SUPERIOR_LOGIN = 'superior_login',
		COLUMN_POSITION = 'position',
		COLUMN_NAME = 'name',
		COLUMN_SURNAME = 'surname',
		COLUMN_STREET = 'street',
		COLUMN_CITY = 'city',
		COLUMN_POSTAL_CODE = 'postal_code',
		COLUMN_BIRTH_NUMBER = 'birth_number',
		COLUMN_CREATED = 'created';

	/** @var Nette\Database\Context */
	private $database;


	public function __construct(Nette\Database\Context $database)
	{
		$this->database = $database;
	}


	/**
	 * Performs an authentication.
	 * @return Nette\Security\Identity
	 * @throws Nette\Security\AuthenticationException
	 */
	public function authenticate(array $credentials)
	{
		list($username, $password) = $credentials;

		$row = $this->database->table(self::TABLE_NAME)->where(self::COLUMN_LOGIN, $username)->fetch();

		if (!$row) {
			throw new Nette\Security\AuthenticationException('The username is incorrect.', self::IDENTITY_NOT_FOUND);

		} elseif (strcmp($password, $row[self::COLUMN_PASSWORD]) != 0 ) {
			throw new Nette\Security\AuthenticationException('The password is incorrect.', self::INVALID_CREDENTIAL);
		}

		$arr = $row->toArray();
		unset($arr[self::COLUMN_PASSWORD]);
		return new Nette\Security\Identity($row[self::COLUMN_LOGIN], $row[self::COLUMN_ROLE], $arr);
	}


	/**
	 * Adds new user.
	 * @param $username
	 * @param $password
	 * @throws DuplicateNameException
	 * @internal param $string
	 * @internal param $string
	 */
	public function add($username, $password, $role)
	{
		try {
			$this->database->table(self::TABLE_NAME)->insert(array(
				self::COLUMN_LOGIN => $username,
				self::COLUMN_PASSWORD => $password,
				self::COLUMN_ROLE => $role
			));
		} catch (Nette\Database\UniqueConstraintViolationException $e) {
			throw new DuplicateNameException;
		}
	}

}

class Role extends Nette\Object
{
	const
			MANAGER = 'manager',
			SUPERVISOR = 'supervisor',
			EMPLOYEE = 'employee';
}


class DuplicateNameException extends \Exception
{}
