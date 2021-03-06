<?php                                                                             
 
/**
 * @file
   Custom CWRC drush scripts.
 */
 
/**
 * Implements hook_drush_command().
 */
function islandora_cwrc_credit_visualization_drush_command()
{
 
  $items = array();
 
  $items['islandora_cwrc_credit_visualization_datastore'] = array(
    'description' => 'Initialize datastore for the CWRC Credit Visualization',
    'examples' => array(
      'drush -u 1 islandora_cwrc_credit_visualization_datastore {pid}'
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_MAX
  );
 
  $items['islandora_cwrc_credit_visualization_datastore_bootstrap'] = array(
    'description' => 'Initialize datastore for the CWRC Credit Visualization',
    'examples' => array(
      'drush -u 1 islandora_cwrc_credit_visualization_datastore_bootstrap'
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_MAX
  );

  $items['islandora_cwrc_credit_visualization_datastore_by_date'] = array(
    'description' => 'Initialize datastore for the CWRC Credit Visualization',
    'examples' => array(
      'drush -u 1 islandora_cwrc_credit_visualization_datastore_by_date "2016-11-12T10:44:27.166Z" '
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_MAX
  );

  return $items;
}
 
 
/**
 * Credit Visualization: update datastore by PID
 *
 */
function drush_islandora_cwrc_credit_visualization_datastore($pid)
{
  try
  {
    module_load_include('inc', 'islandora_cwrc_credit_visualization', 'includes/credit_viz_helpers');

    drush_print("islandora_cwrc_viz_one_PID: $pid");
    islandora_cwrc_viz_one_PID($pid);

  } catch (Exception $e) {
     // print exception
     print $e->getMessage();
  }
}


/**
 * Credit Visualization: bootstrap datastore
 *
 */
function drush_islandora_cwrc_credit_visualization_datastore_bootstrap()
{

  // order by modified date to allow restarting is process crashes
  $query = <<<EOQ
    PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
    PREFIX fedora-view: <info:fedora/fedora-system:def/view#>
    SELECT ?pid ?modified
    FROM <#ri>
    WHERE {
      ?pid fedora-view:lastModifiedDate ?modified ;
      fedora-model:hasModel <info:fedora/fedora-system:FedoraObject-3.0>;
    }
    ORDER BY ?modified
EOQ;

  drush_print($query);

  try
  { 
    module_load_include('inc', 'islandora_cwrc_credit_visualization', 'includes/credit_viz_helpers');

    $connection = islandora_get_tuque_connection();
    $results = $connection->repository->ri->sparqlQuery($query);                
    foreach ($results as $result) 
    {          
      // $documents[] = $result['pid']['value'];
      drush_print($result['pid']['value'] . " " . $result['modified']['value']);
      islandora_cwrc_viz_one_PID($result['pid']['value']);
    }                                        
  } 
  catch (Exception $e) {
     // print exception
     print $e->getMessage();
  }

}

/**
 * Credit Visualization: bootstrap datastore
 *
 */
function drush_islandora_cwrc_credit_visualization_datastore_by_date($startDate = "2016-10-10T15:29:21.374Z")
{

  $query = <<<EOQ
    PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
    PREFIX fedora-view: <info:fedora/fedora-system:def/view#>
    SELECT ?pid ?modified
    FROM <#ri>
    WHERE {
      ?pid fedora-view:lastModifiedDate ?modified ;
      fedora-model:hasModel <info:fedora/fedora-system:FedoraObject-3.0>;
      FILTER (
        ?modified > '$startDate'^^<xml-schema:dateTime>
      )
    }
    ORDER BY ?modified
EOQ;

  drush_print($query);

  try
  { 
    module_load_include('inc', 'islandora_cwrc_credit_visualization', 'includes/credit_viz_helpers');

    $connection = islandora_get_tuque_connection();
    $results = $connection->repository->ri->sparqlQuery($query);                
    foreach ($results as $result) 
    {          
      // $documents[] = $result['pid']['value'];
      drush_print($result['pid']['value'] . " " . $result['modified']['value']);
      islandora_cwrc_viz_one_PID($result['pid']['value']);
    }                                        
  } 
  catch (Exception $e) {
     // print exception
     print $e->getMessage();
  }

}

