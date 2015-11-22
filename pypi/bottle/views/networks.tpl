<!DOCTYPE html>
<html lang="en">

%from random import randint
%from __init__ import connect_to_network

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
  <h2>Table Testing</h2>                        
  <table class="table table-striped table-hover">
    <thead>
      <tr>
        <th>#</th>
        <th>Wireless Network</th>
        <th></th>
      </tr>
    </thead>
    <tbody>

%y = len(ssids)
%input=[]
%for i in range(y):
    %input.append(1)
    %input[i] = ssids[i]
%end  
%for i in range(y):
      <tr id = tableRow data-toggle="modal" data-ssid="{{input[i][0]}}" data-target="#{{input[i][1]}}">
        <td>{{i+1}}</td>
        <td>{{input[i][0]}}</td>
        <td>{{input[i][1]}}</td>
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
        <div class="modal-body" >
          <label for= "pwd">Security key:</label>
          <input type="password" class="form-control" id="pwd">
        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()">OK</button> 
          <button type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()">Cancel</button>
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
        <div class="modal-body">
          <label for= "pwd1">Security key:</label>
          <input type="password" class="form-control" id="pwd">
        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()">OK</button> 
          <button type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()">Cancel</button>
        </div>
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
          <button type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()">OK</button> 
          <button type="button" class="btn btn-default" data-dismiss="modal" onclick="resetFunction()">Cancel</button>
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
      document.getElementById("pwd").value = '';
      document.getElementById("pwd1").value = '';
        }
        
      </script>

  </body>
</html>

