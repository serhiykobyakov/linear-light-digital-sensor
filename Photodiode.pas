unit Photodiode;
{
Linear light sensor with digital output
Photodiode unit
Version 11.09.2022

(c) Serhiy Kobyakov
}


interface

uses
  Classes, SysUtils, dialogs, StdCtrls, Controls, Forms, IniFiles,
  addfunc,
  ArduinoDevice;


type

  { NewDevice_device }

  { Photodiode_device }

  Photodiode_device = Object (_ArduinoDevice)
    private
      fGain: byte;
      function GetGain: byte;
      procedure SetGain(AValue: byte);

    public
      constructor Init(_ComPort: string);
      destructor Done;

      procedure StartAveraging;
      function GetValue: Real;
      procedure SendAndGet;

      property Gain: byte Read GetGain Write SetGain;
  end;


implementation

function Photodiode_device.GetGain: byte;
begin
  Result := fGain;
end;

procedure Photodiode_device.SetGain(AValue: byte);
// set the ADC gain
var
  answer: byte;
begin
  Case AValue of
   1, 2, 4: answer := StrToInt(SendAndGetAnswer('g' + IntToStr(AValue)));
  else
    showmessage(theDeviceID + ':' + LineEnding +
                'procedure ''' + {$I %CURRENTROUTINE%} + ''' failed!' + LineEnding +
                'got value: ' + 'AValue' + LineEnding +
                'Must be: ' + LineEnding +
                '1 for 5V' + LineEnding +
                '2 for 2,5V' + LineEnding +
                '4 for 1,25V');
  end;

  if not ((answer = 1) or
          (answer = 2) or
          (answer = 4)) then
    showmessage(theDeviceID + ':' + LineEnding +
                'procedure ''' + {$I %CURRENTROUTINE%} + ''' failed!' + LineEnding +
                'got value from device: ' + IntToStr(answer) + LineEnding +
                'Must be: ' + LineEnding +
                '1 for 5V' + LineEnding +
                '2 for 2,5V' + LineEnding +
                '4 for 1,25V')
  else fGain := answer;
end;

constructor Photodiode_device.Init(_ComPort: string);
var
  MyForm: TForm;
  MyLabel: TLabel;
//  AppIni: TIniFile;
//  iniFile: string;
  UpperInitStr: string;
begin
// -----------------------------------------------------------------------------
// the device ID string with which it responds to '?'
  theDeviceID := 'Photodiode';
// -----------------------------------------------------------------------------
{
  iniFile := Application.Location + theDeviceID + '.ini';
  If not FileExists(iniFile) then
    begin
      showmessage('File ' + LineEnding + iniFile + LineEnding +
          'procedure ''' + {$I %CURRENTROUTINE%} + ''' failed!' + LineEnding +
          'File ' + iniFile + 'has not been found!' + LineEnding +
          'Please fix it');
      halt(0);
    end;
  }

// make a splash screen
// which shows initialization process
  MyForm := TForm.Create(nil);
  with MyForm do begin
     Caption := theDeviceID + ' initialization...';
     SetBounds(0, 0, 450, 90); Position:=poDesktopCenter; BorderStyle := bsNone;
     MyForm.Color := $00EEEEEE; end;

  MyLabel := TLabel.Create(MyForm);
  with MyLabel do begin
     Autosize := True; Align := alNone; Alignment := taCenter; Parent := MyForm;
     Visible := True; AnchorVerticalCenterTo(MyForm);
     AnchorHorizontalCenterTo(MyForm); end;

  MyForm.Show; MyForm.BringToFront;

  UpperInitStr := 'Initializing ' + theDeviceID + ':' + LineEnding;

  MyLabel.Caption:= UpperInitStr + 'Reading ' + theDeviceID + '.ini...';
  sleepFor(50); // refresh the Label to see the change

// -----------------------------------------------------------------------------
// Read the device variables from ini file:
//  AppIni := TInifile.Create(iniFile);

// device-specific paremeters:


 // AppIni.Free;
// -----------------------------------------------------------------------------

// Use basic device initialization
  MyLabel.Caption:= UpperInitStr + 'Connecting to ' + _ComPort + '...';
  sleepFor(200); // refresh the Label to see the change
  Inherited Init(_ComPort);

  SetGain(1);

  MyLabel.Caption:= UpperInitStr + 'Done!';
  sleepFor(500); // refresh the Label just to see "Done"
  MyForm.Close;
  FreeAndNil(MyForm);
end;

destructor Photodiode_device.Done;
begin
  Inherited Done;
end;

procedure Photodiode_device.StartAveraging;
// tell device to start averaging photodiode current measurements
var
  answer: string;
begin
  answer := Trim(SendCharAndGetAnswer('v'));
  if CompareStr(answer, 'v') <> 0 then
    showmessage('got answer: ' + answer);
end;

function Photodiode_device.GetValue: Real;
// get the photodiode current from device
var
  answer: string;
begin
  answer := Trim(SendCharAndGetAnswer('w'));
  Result := Str2Float(answer)/100;
end;

procedure Photodiode_device.SendAndGet;
// for testing purpouses only
var
  str: string;
begin
  str := InputBox('text to send', 'Please type in what to send', '?');
  showmessage('Got answer: ' + SendAndGetAnswer(str));
end;



end.


