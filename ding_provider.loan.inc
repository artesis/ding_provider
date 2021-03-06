<?php

/**
 * @file
 * Ding loan related classes.
 */

/**
 * Class representing a loan.
 */
class DingProviderLoan extends DingEntityBase {
  /**
   * Renewal succeeded.
   */
  const STATUS_RENEWED = 1;

  /**
   * Renewal failed.
   */
  const STATUS_NOT_RENEWED = 2;

  /**
   * Renewal requested.
   */
  const STATUS_RENEWAL_REQUESTED = 3;

  /**
   * Renewal denied, maximum number of renewals reached.
   */
  const STATUS_RENEWAL_NUM_EXCEEDED = 4;

  /**
   * Renewal denied, reserved by other user.
   */
  const STATUS_RENEWAL_RESERVED = 5;

  /**
   * Id of the reservation. Used as identifier. No assumptions are made about
   * it, however it should be a sting value suitable for values in forms and
   * the like.
   */
  protected $id = DingEntityBase::NULL;

  /**
   * Materials number, optional. Is shown to the user.
   */
  protected $materials_number = DingEntityBase::NULL;

  /**
   * Id of the ding_entity.
   */
  protected $ding_entity_id = DingEntityBase::NULL;

  /**
   * The ding_entity.
   */
  protected $entity = DingEntityBase::NULL;

  /**
   * Fallback display if the loan is an obscure material that is not
   * known by the datawell.
   */
  protected $display_name = DingEntityBase::NULL;

  /**
   * Creation date of reservation. Format: YYYY-MM-DD.
   */
  protected $loan_date = DingEntityBase::NULL;

  /**
   * Loan due date. Format: YYYY-MM-DD.
   */
  protected $expiry = DingEntityBase::NULL;

  /**
   * Whether the loan can be renewed.
   */
  protected $renewable = DingEntityBase::NULL;

  /**
   * True for inter library loans.
   */
  protected $remote_loan = DingEntityBase::NULL;

  /**
   * Notes - some additional information.
   */
  protected $notes = DingEntityBase::NULL;

  /**
   * Message - A message with a reason for why a loan is not renewable
   */
  protected $message = DingEntityBase::NULL;

  public function __construct($id, $data = array()) {
    parent::__construct();
    $this->properties['id'] = $id;
    // Default display name.
    $this->properties['display_name'] = t('Title not available');
    $this->properties['materials_number'] = NULL;
    $properties = array(
      'materials_number',
      'ding_entity_id',
      'entity',
      'display_name',
      'loan_date',
      'expiry',
      'renewable',
      'remote_loan',
      'notes',
      'author',
      'title',
      'publication_year',
      'message',
    );
    $this->_populate($properties, $data);
  }

  public function __get($prop) {
    if (array_key_exists($prop, $this->properties)) {
      if ($this->properties[$prop] === self::NULL) {
        if (($method = 'get' . strtoupper($prop[0]) . substr($prop, 1)) && method_exists($this, $method)) {
          $this->properties[$prop] = call_user_func(array($this, $method));
        }
        else {
          $trace = debug_backtrace();
          trigger_error('No getter defined for ' . $prop .
            ' in ' . $trace[0]['file'] .
            ' on line ' . $trace[0]['line'],
            E_USER_NOTICE);
          return NULL;
        }
      }
      return $this->properties[$prop];
    }

    $trace = debug_backtrace();
    trigger_error('Undefined property via __get(): ' . $prop .
      ' in ' . $trace[0]['file'] .
      ' on line ' . $trace[0]['line'],
      E_USER_NOTICE);

    return NULL;
  }

  function getEntity() {
    if (!isset($this->entity)) {
      if (!empty($this->ding_entity_id)) {
        $this->entity = ding_entity_load($this->ding_entity_id);
      }
      else {
        $this->entity = NULL;
      }
    }
    return $this->entity;
  }

  function getTitle() {
    $this->getEntity();
    if ($this->entity) {
      return $this->title = $this->entity->getTitle();
    }
  }

  function getCreator() {
    $this->getEntity();
    if ($this->entity) {
      return $this->creator = $this->entity->getCreators();
    }
  }

  function getType() {
    $this->getEntity();
    if ($this->entity) {
      return $this->type = $this->entity->getType();
    }
  }

  function getLanguage() {
    $this->getEntity();
    if ($this->entity) {
      return $this->language = $this->entity->getLanguage();
    }
  }
}

/**
 * Parent class for loan errors.
 */
class DingProviderLoanUserError extends DingProviderUserException {
}

