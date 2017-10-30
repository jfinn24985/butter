##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Sat Oct 28 23:45:14 2017
##END:date
##START:preamble
include $(ROOTDIR)/M_sys.mk

##END:preamble
##START:target bouml
BOUML_BOUMLSRC :=  \
	UmlClass.cpp \
	OnInstanceCmd.cpp \
	MiscGlobalCmd.cpp \
	aDirection.cpp \
	UmlArtifact.cpp \
	aRelationKind.cpp \
	UmlBasePackage.cpp \
	UmlDiagram.cpp \
	UmlParameter.cpp \
	UmlClassItem.cpp \
	UmlBaseNode.cpp \
	JavaSettings.cpp \
	UmlBaseClassItem.cpp \
	UmlItem.cpp \
	JavaSettingsCmd.cpp \
	UmlSettings.cpp \
	UmlBaseFormalParameter.cpp \
	UmlOperation.cpp \
	UmlClassView.cpp \
	ClassGlobalCmd.cpp \
	UmlRelation.cpp \
	UmlDeploymentDiagram.cpp \
	aVisibility.cpp \
	UmlBaseDeployment.cpp \
	UmlBaseComponentView.cpp \
	UmlUseCase.cpp \
	UmlBaseUseCase.cpp \
	UmlBaseAttribute.cpp \
	UmlUseCaseView.cpp \
	UmlClassDiagram.cpp \
	UmlBaseUseCaseDiagram.cpp \
	UmlBaseCollaborationDiagram.cpp \
	UmlNcRelation.cpp \
	UmlComponentDiagram.cpp \
	PackageGlobalCmd.cpp \
	UmlBaseOperation.cpp \
	UmlFormalParameter.cpp \
	anItemKind.cpp \
	UmlBaseUseCaseView.cpp \
	UmlBaseComponent.cpp \
	UmlBuiltin.cpp \
	UmlAttribute.cpp \
	UmlCom.cpp \
	UmlBaseDeploymentView.cpp \
	UmlBaseComponentDiagram.cpp \
	UmlBaseClassMember.cpp \
	UmlBaseClassDiagram.cpp \
	UmlTypeSpec.cpp \
	UmlExtraClassMember.cpp \
	UmlCollaborationDiagram.cpp \
	UmlBaseItem.cpp \
	UmlNode.cpp \
	UmlDeploymentView.cpp \
	UmlBaseNcRelation.cpp \
	UmlSettingsCmd.cpp \
	UmlActualParameter.cpp \
	UmlBaseDiagram.cpp \
	UmlBaseDeploymentDiagram.cpp \
	UmlBaseArtifact.cpp \
	UmlBaseClass.cpp \
	UmlPackage.cpp \
	CmdFamily.cpp \
	UmlBaseExtraClassMember.cpp \
	UmlBaseActualParameter.cpp \
	UmlSequenceDiagram.cpp \
	UmlStereotype.cpp \
	UmlClassMember.cpp \
	UmlBaseSequenceDiagram.cpp \
	UmlBaseRelation.cpp \
	UmlBaseClassView.cpp \
	UmlUseCaseDiagram.cpp \
	UmlComponentView.cpp \
	IdlSettingsCmd.cpp \
	UmlComponent.cpp \
	CppSettingsCmd.cpp \
	IdlSettings.cpp \
	CppSettings.cpp
BOUML_BOUML_DINC := $(filter %$(sufdep), $(BOUML_BOUMLSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BOUML_BOUMLSRC:.c=$(sufdep)))
bouml$(suflib): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm
bouml$(suflib) $(BOUML_BOUML_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
BOUML_BOUML_OBJ+=$(filter %$(sufobj), $(BOUML_BOUMLSRC:.cpp=$(sufobj)))
USEROBJ+=$(BOUML_BOUML_OBJ)
bouml$(suflib) : $(BOUML_BOUML_SRC) $(BOUML_BOUML_DINC)
-include $(BOUML_BOUML_DINC)
$(eval $(call do_archive,bouml$(suflib),$(BOUML_BOUML_OBJ)))

all :: bouml$(suflib)
TARGETS+=bouml$(suflib)

$(eval $(call do_install,bouml$(suflib),LIB))

##END:target bouml
