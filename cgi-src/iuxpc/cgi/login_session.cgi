ELF          (    ��  4   $"    4    (      4   4�  4�  �   �            �   �  �                      �   �  |  |      �               \  x      �            �   �         Q�td                          /lib/ld-uClibc.so.0    !                                                                                                  
       	                                                                                                 �   ��         �   ��         �  ��  P     U   ��         �               �   �       	 �   Ć         x   І         �  ܆            �         �  \!      ��'  �         �  x#      ���  x#      ��k    �            �         �  \!      ���   �         �  x#      ���  $�         `   0�         �   <�         �  x#      ���   H�         �  \!      ��    T�         '   `�         ,   l�         ?  x�         7   p�        =                libcgi.so check_default_pass snprintf puts print_file _init __deregister_frame_info sf_strncpy get_pvalue httpcon_auth get_use_captcha_code strcmp _fini hwinfo_get_language_postfix get_si print_http_header httpcon_logout is_mobile_agent _Jv_RegisterClasses __register_frame_info libuserland.so hwinfo_get_product_code get_default_id_password libcrypt.so.0 libnvram.so libshared.so libbcm.so abort libc.so.0 __uClibc_main _edata __bss_start __bss_start__ __bss_end__ __end__ _end  !   !   !   !   !   ! 	  ! 
  !    !   $!   (!   ,!   0!   4!   8!   <!   @!   D!   H!   L!   P!    �����-��L����-���������d�   Ə�	ʌ�d��� Ə�	ʌ�\��� Ə�	ʌ�T��� Ə�	ʌ�L��� Ə�	ʌ�D��� Ə�	ʌ�<��� Ə�	ʌ�4��� Ə�	ʌ�,��� Ə�	ʌ�$��� Ə�	ʌ���� Ə�	ʌ���� Ə�	ʌ���� Ə�	ʌ���� Ə�	ʌ����� Ə�	ʌ����� Ə�	ʌ����� Ə�	ʌ����� Ə�	ʌ����� Ə�	ʌ����� Ə�	ʌ����� Ə�	ʌ����� ��� ���� �� -� -�����-� ��0�����������  Ĉ  p�  @-�,0�� 0��  S��� 0��  S�  
 ������0�� ��  �����\!     x�  @-�40��  S�  
, ��,�����( �� 0��  S���0��  S���3�/����    x�  `!       @-�,��@���� ����� �����x! �  7@-� P�� @�������� 0�� �� �� P����� ��>���x" �  �A-�5�M����@���_�����{����� `����k���m��� �� �� ��x��� �� �� ��t����"��0��@���� p��n�����o������a���r���r�� p������� ����t���  P�  
���v���  P�   p��� �� ��^���  P�  
���r�����  ���O��x��i������@ �� �� ��=���`��h���\��c���X��a���T��_���P��]������/��D��V���@��W���<��U���8��V���@���  P�  
(��K���$��O��K�����_��H�����F�����D�������>�����?��� ��=���/�������6��������3������4������2������/������O��*���  W�������%������&����� ������������1��  W������q����!��1���� �0����������������������������	��������  V�  
t���� ������h������d��O������\�������/����P������� ������8��������8������4������0������t ������p ������������������������������������������������ ������  ��5ލ�����`�  c�  i�  p�  r�  y�  ��  ލ  ��  �  #�  �  ��  s�  �  ��  ܔ  J�  R�  Z�  ��  ϕ  ��  '�  x�  ��  �  K�  r�  З  �  \�  ��  �  D�  P�  Y�  #�  I�  ��  ��  ޙ  �  A�  u�  ��  �  `�  �  �  ,�  ��  �  �  �  -�  /�  X�  �  o�  ��  ��  �����-��L���/images2 %s/%s 초기암호:admin(변경필요) text 888888 password 000000 pi %s %s logout 1 <HTML> <SCRIPT>document.location.href="/sess-bin/login.cgi"</SCRIPT> </HTML> <HTML><HEAD><TITLE>%s</TITLE>
 /home/httpd/js/login_session.js <SCRIPT type="text/javascript"> <!-- function LoginProcess()
		{
			if(document.form.captcha_on.value == '1')				document.form.captcha_file.value=iframe_captcha.document.captcha_form.captcha_file.value;
			document.form.submit();
		} function ChangeCaptchaUse(flag)
        {
                if(flag==0)
                {
                        with(document)
                        {
                                getElementById("captchatr1").style.display = "none";
                                getElementById("captchatr2").style.display = "none";
                                getElementById("captchatr3").style.display = "none";
                        }
                }
                else
                        ChangeCaptchaInputBg();
        } function ChangeCaptchaInputBg(flag)
        {
                if(flag=="clear")
                        document.form.captcha_code.style.cssText="background-image:url();width:255px; height:21px;";
                else
                        document.form.captcha_code.style.cssText="background-image:url(%s/login_str_captcha_bg.%s.gif);width:255px; height:21px;";
        }
 function FocusPassword()
	{
        var F = document.form;
        if(F.passwd.value == F.default_passwd.value)
        {
		document.getElementById("passwd_td").innerHTML = '<input type=password name=passwd CLASS=login_input TABINDEX=2>';
                document.form.passwd.value = '';
                document.form.passwd.style.color = "#000000";
                document.form.passwd.focus();
        }
	} function RenewCaptchaImage()
        {
                document.getElementById("iframe_captcha").contentWindow.location.href="/sess-bin/captcha.cgi";
        }
	//-->
	</SCRIPT> /home/httpd/login_session.css <meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=2.0; user-scalable=0;" /> </HEAD> <BODY > <FORM NAME="form" METHOD="post" ACTION="/sess-bin/login_handler.cgi"> <INPUT TYPE=hidden NAME="init_status" value=1> <INPUT TYPE=hidden NAME="captcha_on" value=%d>
 <INPUT TYPE=hidden NAME="captcha_file"> <TABLE CELLPADDING=0 CELLSPACING=0 CLASS=navi_login_table ALIGN=CENTER border=0> <TR><TD><IMG SRC="%s/login_title.%s.gif" BORDER=0></TD></TR>
 <TR><TD BACKGROUND="%s/login_main_bg.gif" STYLE="vertical-align:top;padding:0 0 0 3px;">
 <TABLE CELLPADDING=0 CELLSPACING=0 WIDTH=258 ALIGN=CENTER> <TR><TD COLSPAN=2 HEIGHT=10></TD></TR> <TR><TD WIDTH=70 STYLE="padding:0 0 0 1px;"><IMG SRC="%s/login_str_id.%s.gif" BORDER=0></TD>
 <TD><INPUT type=text name=username CLASS=login_input TABINDEX=1 value="%s" onkeydown="if (event.keyCode == 13) LoginProcess();"></TD></TR>
 <TR><TD COLSPAN=2 HEIGHT=3></TD></TR> <TR><TD WIDTH=70 STYLE="padding:0 0 0 1px;"><IMG SRC="%s/login_str_passwd.%s.gif" BORDER=0></TD>
 <TD ID='passwd_td'> <INPUT type=%s name=passwd CLASS=login_input TABINDEX=2 onfocus="FocusPassword();" value="%s" style="color:#%s" onkeydown="if (event.keyCode == 13) LoginProcess();"></TD></TR>
 <input type=hidden name="default_passwd" value="%s"> <TR><TD COLSPAN=2 HEIGHT=8></TD></TR> <TR ID=captchatr1><TD COLSPAN=2><INPUT type=text name=captcha_code CLASS=login_input STYLE="width:255px; height:21px;" autocomplete='off' autocorrect= 'off' autocapitalize = 'off' spellcheck = 'false' ONFOCUS="ChangeCaptchaInputBg('clear');" TABINDEX=3 onkeydown="if (event.keyCode == 13) LoginProcess();"></TD></TR> <TR ID=captchatr2><TD COLSPAN=2 HEIGHT=5></TD></TR> <TR ID=captchatr3><TD COLSPAN=2 HEIGHT=72> <TABLE CELLPADDING=0 CELLSPACING=0 HEIGHT=72 CLASS="login_input"> <TR><TD WIDTH=25><IMG SRC="%s/login_bt_refresh.%s.gif" BORDER=0 ONCLICK="RenewCaptchaImage();" STYLE="cursor:pointer;"></TD>
 <TD><IFRAME NAME=iframe_captcha ID=iframe_captcha SRC="/sess-bin/captcha.cgi" WIDTH=201 HEIGHT=70 FRAMEBORDER=no SCROLLING=no></IFRAME></TD></TR> </TABLE></TD></TR> <TR><TD COLSPAN=2 HEIGHT=12></TD></TR> <TR><TD COLSPAN=2 HEIGHT=50><IMG style="cursor:pointer" SRC="%s/login_bt.%s.gif" TABINDEX=4 ID="submit_bt" onclick="LoginProcess();"  ></TD></TR>
 <TR><TD HEIGHT=10 BACKGROUND="%s/login_sess_back_info.gif"></TD></TR></TABLE>
 </FORM> </BODY> with(document) { ChangeCaptchaUse(form.captcha_on.value); form.username.focus(); //-->                                                                                                                                                                                                                                                                                                                                                                                                            �  ̇                    W     e     q     ~     �     p�     �                            �     �     ؁  
   �                  �     �            ȅ                                                            ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��  ��          A*   aeabi     7-A 
A	, .shstrtab .interp .hash .dynsym .dynstr .rel.plt .init .text .fini .rodata .eh_frame .init_array .fini_array .jcr .dynamic .got .data .bss .ARM.attributes                                                   �  �                              �    �                         ؁  �                !         �  �  �                 )   	      ȅ  �  �               2         p�  p                    -         ��  �                  8         ��  �  t                 >         �                      D      2   �    a                L         x�  x                    V                                  b                                n                                s              �                |         �  �   `                 �         T! T!                    �         \! \!                   �     p        \!  +                                �!  �                  