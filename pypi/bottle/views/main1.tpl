<!DOCTYPE html>
<html lang="en">
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
    <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="//html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->

    <!-- Le fav and touch icons -->
    <!--<link rel="shortcut icon" href="/images/favicon.ico">
    <link rel="apple-touch-icon" href="/images/apple-touch-icon.png">
    <link rel="apple-touch-icon" sizes="72x72" href="/images/apple-touch-icon-72x72.png">
    <link rel="apple-touch-icon" sizes="114x114" href="/images/apple-touch-icon-114x114.png">-->
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
%from random import randint
%from __init__ import connect_to_network
%x = randint(0,9)
%y = len(List1)
%input=[]
%for i in range(y):
    %input.append(1)
    %input[i] = List1[i]
%end  
%for i in range(y):
      <tr id = tableRow data-toggle="modal" data-SSID="{{input[i][0]}}" data-target="#{{input[i][1]}}" >
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
          <button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>
        </div>
      </div>
    </div>
  </div>
  
  <div class="modal fade" id="connecting" role="dialog">
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
          <button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>
        </div>
      </div>
      
    </div>
  </div>
  
  
</div>
 

    <!-- Le javascript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <!-- But really should be async loaded -->
      <script src="/js/jquery.min.js"></script>
      <script src="/js/bootstrap.min.js"></script>
      <script>
      function resetFunction() {
      document.getElementById("pwd").value = '';
        }
      
      var ssid = (document.getElementById(tableRow)).trID.dataset.SSID;
      $('#WEP').modal('hide')
      $('#myModal').on('hidden', function () {
         $('#myModalNew').modal('show')
        })
        
      </script>
  

  
 

  </body>
</html>
