<!DOCTYPE html>
<html lang="en">

%from random import randint
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <!-- Le styles -->
    <link href="/css/bootstrap.min.css" rel="stylesheet">
    <style>
      body {
        padding-top: 60px; /* 60px to make the container go all the way to the bottom of the topbar */
      }
    </style>
  </head>

  <body>    
   <div class="container">
  <h2>Wifi Access Points</h2>
  <table class="table table-striped table-hover">
    <thead>
      <tr>
        <th>#</th>
        <th>Wireless Network</th>
        <th></th>
      </tr>
    </thead>
    <tbody>

%length_ssid = len(ssids)
%for i in range(length_ssid):
      <tr id = tableRow data-toggle="modal" data-target="#{{ssids[i][1]}}"  onclick="mSSID({{i}})">
        <td>{{i+1}}</td>
        <td id= "tableCol_SSID{{i}}">{{ssids[i][0]}}</td>
        <td id= "tableCol_type{{i}}">{{ssids[i][1]}}</td>
      </tr>
%end
    </tbody>
  </table>

<!-- Modal -->

<div class="modal fade" id="WEP" role="dialog">
    <div class="modal-dialog">

      <!-- Modal content-->
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Type the network security key</h4>
        </div>

        <form action ="/" method="POST" >
        <div class="modal-body" >
          <label for= "pwdWEP">Security key:</label>
          <input type="password" class="form-control" id="pwdWEP" name="pwdWEP">
        </div>
        <div class="modal-footer">
          <input name="fWEP" id="idWEP">
          <input name="sub_WEP" type="submit" class="btn btn-default" value="Connect">
          <input type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()" value ="Cancel">
          </form>
         </div>
      </div>
    </div>
  </div>


  <div class="modal fade" id="WPA" role="dialog">
    <div class="modal-dialog">

      <!-- Modal content-->
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Type the network security key</h4>
        </div>

        <form action ="/" method="POST" >
        <div class="modal-body">
          <label for= "pwdWPA">Security key:</label>
          <input type="password" class="form-control" id="pwdWPA" name="pwdWPA">
        </div>
        <div class="modal-footer">

          <input name="fWPA" id="idWPA">
          <input name="sub_WPA" type="submit" class="btn btn-default" value="Connect">
          <input type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()" value ="Cancel">
          </form>
        </div>-
      </div>

    </div>
  </div>


  <div class="modal fade" id="OPEN" role="dialog">
    <div class="modal-dialog">

      <!-- Modal content-->
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title" >Would you like to connect?</h4>
        </div>
        <div class="modal-body">
        </div>
        <div class="modal-footer">
          <form action ="/" method="POST" >
          <input name="fOPEN" id="idOPEN">
          <input type="submit" name="sub_OPEN" value="Connect" class="btn btn-default">
          <input type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()" value ="Cancel">
          </form>
        </div>
      </div>
    </div>
  </div>

  
</div>
 

    <!-- Le javascript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->

      <script src="/js/jquery.min.js"></script>
      <script src="/js/bootstrap.min.js"></script>
       <script>

       function resetFunction() {
          document.getElementById("pwdWEP").value = '';
          document.getElementById("pwdWPA").value = '';
        }

      function mSSID(indexSSID){
          if(document.getElementById("tableCol_type"+indexSSID).innerHTML=='OPEN'){
            document.getElementById("idOPEN").value = document.getElementById("tableCol_SSID"+indexSSID).innerHTML;
            }
          else if( document.getElementById("tableCol_type"+indexSSID).innerHTML== 'WPA'){
            document.getElementById("idWPA").value = document.getElementById("tableCol_SSID"+indexSSID).innerHTML;
            }
          else if( document.getElementById("tableCol_type"+indexSSID).innerHTML == 'WEP'){
            document.getElementById("idWEP").value = document.getElementById("tableCol_SSID"+indexSSID).innerHTML;
            }
       }

      </script>

  </body>
</html>

